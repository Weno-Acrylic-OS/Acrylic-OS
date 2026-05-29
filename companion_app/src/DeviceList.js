import React from 'react';
import { FlatList, Text, TouchableOpacity, StyleSheet } from 'react-native';

const DeviceList = ({ devices, onDevicePress }) => {
  const renderItem = ({ item }) => (
    <TouchableOpacity style={styles.item} onPress={() => onDevicePress(item)}>
      <Text>{item.name || 'Unknown Device'}</Text>
      <Text>{item.id}</Text>
    </TouchableOpacity>
  );

  return (
    <FlatList
      data={devices}
      renderItem={renderItem}
      keyExtractor={item => item.id}
    />
  );
};

const styles = StyleSheet.create({
  item: {
    padding: 10,
    borderBottomWidth: 1,
    borderBottomColor: '#ccc',
  },
});

export default DeviceList;
