import React, {useState} from 'react';
import {StyleSheet, TouchableOpacity, Text, View} from 'react-native';
import {FontAwesomeIcon} from '@fortawesome/react-native-fontawesome';
import {faHome} from '@fortawesome/free-solid-svg-icons';
import {useWebSockets} from '../../Context/WebSocketContext';
import useIsMounted from 'ismounted';
var Spinner = require('react-native-spinkit');

const ConnectButton = ({ip, port}) => {
  const {initWebSocket, connectionStatus} = useWebSockets();
  const [loading, setLoading] = useState(false);
  const [caption, setCaption] = useState(
    connectionStatus ? 'Connected' : 'Connect',
  );
  const isMounted = useIsMounted();

  const displayConnectionState = (state) => {
    if (isMounted.current) {
      switch (state) {
        case 'connect':
          setCaption('Connect');
          setLoading(false);
          break;
        case 'connecting':
          setCaption('Connecting');
          setLoading(true);
          break;
        case 'connected':
          setCaption('Connected');
          setLoading(false);
          break;
        case 'failed':
          setCaption('Connection Failed');
          setLoading(false);
          setTimeout(() => {
            displayConnectionState('connect');
          }, 2000);
          break;
      }
    }
  };

  return (
    <View style={styles.ConnectButtonContainer}>
      <TouchableOpacity
        onPress={() => {
          displayConnectionState('connecting');
          initWebSocket(ip, port, (result) => {
            if (result) {
              displayConnectionState('connected');
            } else {
              displayConnectionState('failed');
            }
          });
        }}
        style={styles.subContainer}>
        <Text style={styles.text}>{caption}</Text>
        <Spinner
          isVisible={loading ? true : false}
          size={18}
          type={'WanderingCubes'}
          color={'#FFFFFF'}
          style={styles.loadSpinner}
        />
      </TouchableOpacity>
    </View>
  );
};

const styles = StyleSheet.create({
  ConnectButtonContainer: {
    marginTop: 10,
    marginHorizontal: 10,
    backgroundColor: '#ff1053',
    alignSelf: 'stretch',
    borderColor: 'white',
    borderWidth: 1,
    paddingHorizontal: 4,
    paddingVertical: 2,
    borderRadius: 6,
  },
  subContainer: {
    flexDirection: 'row',
    justifyContent: 'center',
  },
  ConnectButton: {
    color: 'white',
  },
  text: {
    color: 'white',
    fontSize: 23,
    textAlign: 'center',
  },
  loadSpinner: {
    marginHorizontal: 10,
    position: 'relative',
    top: 7,
  },
});

export default ConnectButton;