/* eslint-disable react-hooks/exhaustive-deps */
import React, {useRef, useEffect, useState} from 'react';
import {StyleSheet, View, Text, StatusBar, AppState} from 'react-native';

import Header from '../components/HomeScreen/Header';
import ToggleButton from '../components/HomeScreen/ToggleButton';
import ConnectionStatus from '../components/HomeScreen/ConnectionStatus';
import {useWebSockets} from '../Context/WebSocketContext';

const HomeScreen = ({navigation}) => {
  const {closeWebSocket} = useWebSockets();

  useEffect(() => {
    StatusBar.setBarStyle('light-content', true);
    StatusBar.setBackgroundColor('#E80042');
    AppState.addEventListener('change', (state) => {
      if (state === 'background') {
        closeWebSocket();
      }
    });
  }, []);

  const navigateToSettings = () => {
    navigation.navigate('Settings');
  };

  return (
    <View style={styles.container}>
      <Header title="LED Controller" navigateToSettings={navigateToSettings} />
      <View style={styles.matrixContainer}>
        <View style={styles.buttonMatrix}>
          {Array(8)
            .fill()
            .map((item, i) => (
              <ToggleButton index={i} key={i} />
            ))}
        </View>
      </View>
      <ConnectionStatus />
    </View>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    flexDirection: 'column',
    backgroundColor: '#FFFFFF',
  },
  matrixContainer: {
    flex: 1,
    alignSelf: 'center',
    justifyContent: 'center',
    flexDirection: 'column',
  },
  buttonMatrix: {
    marginHorizontal: 5,
    flexDirection: 'row',
    flexWrap: 'wrap',
  },
  mainText: {
    fontSize: 22,
    fontWeight: 'normal',
  },
});

export default HomeScreen;
