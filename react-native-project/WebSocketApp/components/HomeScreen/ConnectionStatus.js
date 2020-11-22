import React from 'react';
import {View, Text, StyleSheet} from 'react-native';

import {useSettings} from '../../Context/SettingsContext';
import {useWebSockets} from '../../Context/WebSocketContext';

const ConnectionStatus = () => {
  const {getSettings, updateSettings} = useSettings();
  const {connectionStatus} = useWebSockets();
  return (
    <View style={styles.infoContainer}>
      <View style={styles.statusContainer}>
        <Text style={styles.statusTextCaption}>Status: </Text>
        <Text style={styles.statusText}>
          {connectionStatus ? 'connected' : 'disconnected'}
        </Text>
      </View>
      <View style={styles.statusContainer}>
        <Text style={styles.statusTextCaption}>IP: </Text>
        <Text style={styles.statusText}>{getSettings().ipAddress}</Text>
      </View>
    </View>
  );
};

const styles = StyleSheet.create({
  statusTextCaption: {
    fontSize: 11,
    fontWeight: 'bold',
    color: '#000000',
  },
  infoContainer: {
    flex: 1,
    flexDirection: 'column',
    right: 0,
    bottom: 0,
    position: 'absolute',
    margin: 10,
  },
  statusText: {
    fontSize: 11,
    fontWeight: 'bold',
    color: '#ff1053',
  },
  statusContainer: {
    flex: 1,
    flexDirection: 'row',
    margin: 0,
  },
});

export default ConnectionStatus;
