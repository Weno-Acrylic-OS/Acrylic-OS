import React, { useState, useEffect } from 'react';
import {
  SafeAreaView,
  StyleSheet,
  View,
  Button,
  Text,
  PermissionsAndroid,
  Platform,
} from 'react-native';
import { BleManager, Device } from 'react-native-ble-plx';
import { NavigationContainer } from '@react-navigation/native';
import { createBottomTabNavigator } from '@react-navigation/bottom-tabs';
import DeviceList from './src/DeviceList';
import bluetoothManager from './src/BLEManager';
import GoalsScreen from './src/GoalsScreen';
import MedicalIdScreen from './src/MedicalIdScreen';

const HEART_RATE_SERVICE_UUID = '0000180d-0000-1000-8000-00805f9b34fb';
const HEART_RATE_CHARACTERISTIC_UUID = '00002a37-0000-1000-8000-00805f9b34fb';

const WENOFIT_SERVICE_UUID = '6e400001-b5a3-f393-e0a9-e50e24dcca9e';
const STEPS_CHARACTERISTIC_UUID = '6e400002-b5a3-f393-e0a9-e50e24dcca9e';
const SLEEP_CHARACTERISTIC_UUID = '6e400003-b5a3-f393-e0a9-e50e24dcca9e';
const MEDICAL_ID_CHARACTERISTIC_UUID = '6e400004-b5a3-f393-e0a9-e50e24dcca9e';
const DEVICE_CONTROL_CHARACTERISTIC_UUID = '6e400005-b5a3-f393-e0a9-e50e24dcca9e';

interface SleepData {
  light: number;
  deep: number;
  rem: number;
}

const Tab = createBottomTabNavigator();

const DeviceScreen = ({
  status,
  connectedDevice,
  isScanning,
  startScan,
  disconnectFromDevice,
  devices,
  connectToDevice,
  heartRate,
  steps,
  sleepData
}) => {
  const findMyWatch = () => {
    const command = JSON.stringify({ command: 'find_my_watch', action: 'start' });
    bluetoothManager.writeCharacteristic(
      WENOFIT_SERVICE_UUID,
      DEVICE_CONTROL_CHARACTERISTIC_UUID,
      command
    );
  };

  return (
    <SafeAreaView style={styles.container}>
      <View style={styles.header}>
        <Text style={styles.status}>{status}</Text>
        {connectedDevice ? (
          <Button title="Disconnect" onPress={disconnectFromDevice} />
        ) : (
          <Button title={isScanning ? 'Scanning...' : 'Scan for Devices'} onPress={startScan} disabled={isScanning} />
        )}
      </View>
      {connectedDevice ? (
        <View style={styles.connectedView}>
          <Text>Connected to {connectedDevice.name}</Text>
          <Text style={styles.dataText}>❤️ {heartRate} BPM</Text>
          <Text style={styles.dataText}>👣 {steps} Steps</Text>
          {sleepData && (
            <Text style={styles.dataText}>
              😴 Sleep: {Math.floor(sleepData.light / 60)}h {sleepData.light % 60}m (Light), {Math.floor(sleepData.deep / 60)}h {sleepData.deep % 60}m (Deep), {Math.floor(sleepData.rem / 60)}h {sleepData.rem % 60}m (REM)
            </Text>
          )}
          <View style={styles.buttonContainer}>
            <Button title="Find my Watch" onPress={findMyWatch} />
          </View>
        </View>
      ) : (
        <DeviceList devices={devices} onDevicePress={connectToDevice} />
      )}
    </SafeAreaView>
  );
};

const App = () => {
  const [devices, setDevices] = useState<Device[]>([]);
  const [isScanning, setIsScanning] = useState(false);
  const [connectedDevice, setConnectedDevice] = useState<Device | null>(null);
  const [status, setStatus] = useState('Idle');
  const [heartRate, setHeartRate] = useState(0);
  const [steps, setSteps] = useState(0);
  const [sleepData, setSleepData] = useState<SleepData | null>(null);

  const addDevice = (device: Device) => {
    if (device.name) {
      setDevices((prevDevices) => {
        if (prevDevices.find((d) => d.id === device.id)) {
          return prevDevices;
        }
        return [...prevDevices, device];
      });
    }
  };

  const startScan = async () => {
    const hasPermissions = await bluetoothManager.requestPermissions();
    if (hasPermissions) {
      setIsScanning(true);
      setStatus('Scanning...');
      setDevices([]);
      bluetoothManager.scanForDevices(addDevice);
      setTimeout(() => {
        setIsScanning(false);
        setStatus('Idle');
        bluetoothManager.stopScan();
      }, 5000);
    }
  };

  const connectToDevice = async (device: Device) => {
    setStatus(`Connecting to ${device.name}...`);
    bluetoothManager.stopScan();
    setIsScanning(false);
    const connected = await bluetoothManager.connectToDevice(device.id);
    if (connected) {
      setConnectedDevice(connected);
      setStatus(`Connected to ${device.name}`);
      
      bluetoothManager.monitorCharacteristic(HEART_RATE_SERVICE_UUID, HEART_RATE_CHARACTERISTIC_UUID, (data) => {
        const heartRateValue = data.charCodeAt(1);
        setHeartRate(heartRateValue);
      });

      bluetoothManager.monitorCharacteristic(WENOFIT_SERVICE_UUID, STEPS_CHARACTERISTIC_UUID, (data) => {
        const stepsValue = data.charCodeAt(0) | (data.charCodeAt(1) << 8) | (data.charCodeAt(2) << 16) | (data.charCodeAt(3) << 24);
        setSteps(stepsValue);
      });

      bluetoothManager.monitorCharacteristic(WENOFIT_SERVICE_UUID, SLEEP_CHARACTERISTIC_UUID, (data) => {
        try {
          const sleepValue = JSON.parse(data);
          setSleepData(sleepValue);
        } catch (e) {
          console.error("Failed to parse sleep data", e);
        }
      });
    } else {
      setStatus('Connection failed');
    }
  };

  const disconnectFromDevice = async () => {
    if (connectedDevice) {
      await bluetoothManager.disconnectFromDevice(connectedDevice.id);
      setConnectedDevice(null);
      setHeartRate(0);
      setSteps(0);
      setSleepData(null);
      setStatus('Disconnected');
    }
  };

  return (
    <NavigationContainer>
      <Tab.Navigator>
        <Tab.Screen name="Device">
          {() => (
            <DeviceScreen
              status={status}
              connectedDevice={connectedDevice}
              isScanning={isScanning}
              startScan={startScan}
              disconnectFromDevice={disconnectFromDevice}
              devices={devices}
              connectToDevice={connectToDevice}
              heartRate={heartRate}
              steps={steps}
              sleepData={sleepData}
            />
          )}
        </Tab.Screen>
        <Tab.Screen name="Goals" component={GoalsScreen} />
        <Tab.Screen name="Medical ID" component={MedicalIdScreen} />
      </Tab.Navigator>
    </NavigationContainer>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
  },
  header: {
    padding: 10,
  },
  status: {
    textAlign: 'center',
    marginBottom: 10,
  },
  connectedView: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  dataText: {
    fontSize: 20,
    marginTop: 20,
  },
  buttonContainer: {
    marginTop: 20,
  }
});

export default App;
