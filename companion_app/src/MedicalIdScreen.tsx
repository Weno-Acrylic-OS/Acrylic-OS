import React, { useState, useEffect } from 'react';
import { View, Text, TextInput, Button, StyleSheet, Alert } from 'react-native';
import bluetoothManager from './BLEManager';

const WENOFIT_SERVICE_UUID = '6e400001-b5a3-f393-e0a9-e50e24dcca9e';
const MEDICAL_ID_CHARACTERISTIC_UUID = '6e400004-b5a3-f393-e0a9-e50e24dcca9e';

const MedicalIdScreen = () => {
  const [name, setName] = useState('');
  const [dob, setDob] = useState('');
  const [conditions, setConditions] = useState('');
  const [allergies, setAllergies] = useState('');
  const [emergencyContact, setEmergencyContact] = useState('');

  useEffect(() => {
    const fetchMedicalId = async () => {
      const data = await bluetoothManager.readCharacteristic(
        WENOFIT_SERVICE_UUID,
        MEDICAL_ID_CHARACTERISTIC_UUID
      );
      if (data) {
        try {
          const medicalId = JSON.parse(data);
          setName(medicalId.name || '');
          setDob(medicalId.dob || '');
          setConditions(medicalId.conditions || '');
          setAllergies(medicalId.allergies || '');
          setEmergencyContact(medicalId.emergencyContact || '');
        } catch (e) {
          console.error("Failed to parse medical ID data", e);
        }
      }
    };

    fetchMedicalId();
  }, []);

  const handleSave = async () => {
    const medicalId = {
      name,
      dob,
      conditions,
      allergies,
      emergencyContact,
    };
    const data = JSON.stringify(medicalId);
    await bluetoothManager.writeCharacteristic(
      WENOFIT_SERVICE_UUID,
      MEDICAL_ID_CHARACTERISTIC_UUID,
      data
    );
    Alert.alert("Medical ID Saved", "Your medical ID has been saved to your device.");
  };

  return (
    <View style={styles.container}>
      <Text style={styles.label}>Name</Text>
      <TextInput style={styles.input} value={name} onChangeText={setName} />

      <Text style={styles.label}>Date of Birth</Text>
      <TextInput style={styles.input} value={dob} onChangeText={setDob} placeholder="YYYY-MM-DD" />

      <Text style={styles.label}>Medical Conditions</Text>
      <TextInput style={styles.input} value={conditions} onChangeText={setConditions} />

      <Text style={styles.label}>Allergies & Reactions</Text>
      <TextInput style={styles.input} value={allergies} onChangeText={setAllergies} />

      <Text style={styles.label}>Emergency Contact</Text>
      <TextInput style={styles.input} value={emergencyContact} onChangeText={setEmergencyContact} />

      <Button title="Save" onPress={handleSave} />
    </View>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    padding: 16,
  },
  label: {
    fontSize: 16,
    fontWeight: 'bold',
    marginTop: 8,
  },
  input: {
    height: 40,
    borderColor: 'gray',
    borderWidth: 1,
    borderRadius: 4,
    paddingHorizontal: 8,
    marginBottom: 16,
  },
});

export default MedicalIdScreen;