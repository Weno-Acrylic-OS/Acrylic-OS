import React, { useState, useEffect } from 'react';
import { View, Text, StyleSheet, FlatList } from 'react-native';
import bluetoothManager from './BLEManager';

const WENOFIT_SERVICE_UUID = '6e400001-b5a3-f393-e0a9-e50e24dcca9e';
const GOALS_CHARACTERISTIC_UUID = '6e400007-b5a3-f393-e0a9-e50e24dcca9e';

interface Goal {
  type: number;
  description: string;
  target_value: number;
  current_value: number;
  xp_reward: number;
  is_complete: boolean;
}

const ProgressBar = ({ progress }) => {
  return (
    <View style={styles.progressBar}>
      <View style={{ width: `${progress * 100}%`, ...styles.progress }} />
    </View>
  );
};

const GoalsScreen = () => {
  const [goals, setGoals] = useState<Goal[]>([]);

  useEffect(() => {
    const subscription = bluetoothManager.monitorCharacteristic(
      WENOFIT_SERVICE_UUID,
      GOALS_CHARACTERISTIC_UUID,
      (data) => {
        try {
          const newGoals = JSON.parse(data);
          setGoals(newGoals);
        } catch (e) {
          console.error("Failed to parse goals data", e);
        }
      },
    );

    return () => {
      if (subscription) {
        subscription.remove();
      }
    };
  }, []);

  const renderItem = ({ item }) => (
    <View style={styles.goalItem}>
      <Text style={styles.goalDescription}>{item.description}</Text>
      <Text>{item.current_value} / {item.target_value}</Text>
      <ProgressBar progress={item.current_value / item.target_value} />
    </View>
  );

  return (
    <View style={styles.container}>
      <FlatList
        data={goals}
        renderItem={renderItem}
        keyExtractor={(item) => item.type.toString()}
        ListHeaderComponent={<Text style={styles.title}>Your Goals</Text>}
      />
    </View>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    padding: 10,
  },
  title: {
    fontSize: 24,
    fontWeight: 'bold',
    marginBottom: 20,
  },
  goalItem: {
    marginBottom: 20,
  },
  goalDescription: {
    fontSize: 18,
  },
  progressBar: {
    height: 20,
    width: '100%',
    backgroundColor: '#e0e0e0',
    borderRadius: 10,
    marginTop: 5,
  },
  progress: {
    height: '100%',
    backgroundColor: 'green',
    borderRadius: 10,
  },
});

export default GoalsScreen;
