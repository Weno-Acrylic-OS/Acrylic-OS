import { BleManager, Device } from 'react-native-ble-plx';
import { PermissionsAndroid, Platform } from 'react-native';
import { atob, btoa } from 'react-native-quick-base64';

class BluetoothManager {
  constructor() {
    this.manager = new BleManager();
    this.device = null;
  }

  async requestPermissions() {
    if (Platform.OS === 'android') {
      const granted = await PermissionsAndroid.request(
        PermissionsAndroid.PERMISSIONS.ACCESS_FINE_LOCATION,
        {
          title: 'Location Permission',
          message: 'This app needs access to your location to scan for Bluetooth devices.',
          buttonNeutral: 'Ask Me Later',
          buttonNegative: 'Cancel',
          buttonPositive: 'OK',
        },
      );
      return granted === PermissionsAndroid.RESULTS.GRANTED;
    }
    return true;
  }

  scanForDevices(onDeviceFound) {
    this.manager.startDeviceScan(null, null, (error, device) => {
      if (error) {
        console.error(error);
        return;
      }
      onDeviceFound(device);
    });
  }

  stopScan() {
    this.manager.stopDeviceScan();
  }

  async connectToDevice(deviceId) {
    try {
      const device = await this.manager.connectToDevice(deviceId);
      console.log('Connected to', device.name);
      this.device = device;
      await device.discoverAllServicesAndCharacteristics();
      const services = await device.services();
      console.log('Discovered services:', services);
      return device;
    } catch (error) {
      console.error('Connection error:', error);
    }
  }

  async disconnectFromDevice(deviceId) {
    try {
      await this.manager.cancelDeviceConnection(deviceId);
      this.device = null;
      console.log('Disconnected from', deviceId);
    } catch (error) {
      console.error('Disconnection error:', error);
    }
  }

  monitorCharacteristic(serviceUUID, characteristicUUID, onData) {
    if (!this.device) {
      return null;
    }
    return this.manager.monitorCharacteristicForDevice(
      this.device.id,
      serviceUUID,
      characteristicUUID,
      (error, characteristic) => {
        if (error) {
          console.error(error);
          return;
        }
        const decoded = atob(characteristic.value);
        onData(decoded);
      },
    );
  }

  async writeCharacteristic(serviceUUID, characteristicUUID, data) {
    if (!this.device) {
      console.error("No device connected");
      return;
    }
    try {
      const encodedData = btoa(data);
      await this.device.writeCharacteristicWithResponseForService(
        serviceUUID,
        characteristicUUID,
        encodedData
      );
      console.log("Wrote data:", data);
    } catch (error) {
      console.error("Failed to write characteristic:", error);
    }
  }

  async readCharacteristic(serviceUUID, characteristicUUID) {
    if (!this.device) {
      console.error("No device connected");
      return null;
    }
    try {
      const characteristic = await this.device.readCharacteristicForService(
        serviceUUID,
        characteristicUUID
      );
      const decodedData = atob(characteristic.value);
      return decodedData;
    } catch (error) {
      console.error("Failed to read characteristic:", error);
      return null;
    }
  }
}

const bluetoothManager = new BluetoothManager();
export default bluetoothManager;
