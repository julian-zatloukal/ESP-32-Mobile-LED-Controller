import React, {useEffect, useState} from 'react';
import {Text, StyleSheet, TouchableOpacity} from 'react-native';
import {useWebSockets} from '../../Context/WebSocketContext';

const ToggleButton = ({index}) => {
  const {ledArray, setLedArray} = useWebSockets();

  const handlePress = () => {
    setLedArray(Array.from(ledArray, (x, i) => (i === index ? Number(!x) : x)));
  };

  return (
    <TouchableOpacity
      style={[
        styles.container,
        {backgroundColor: ledArray[index] ? '#ff1053' : 'white'},
        {borderWidth: ledArray[index] ? 0 : 2},
      ]}
      onPress={handlePress}>
      <Text
        style={[styles.text, {color: ledArray[index] ? 'white' : '#ff1053'}]}>
        {index}
      </Text>
    </TouchableOpacity>
  );
};

ToggleButton.defaultProps = {
  index: 0,
};

const styles = StyleSheet.create({
  container: {
    height: 70,
    width: 70,
    padding: 15,
    backgroundColor: '#ff1053',
    borderRadius: 5,
    justifyContent: 'center',
    alignItems: 'center',
    margin: 5,
    flex: 1,
    flexShrink: 0,
    flexBasis: '22%',
    borderColor: '#ff1053',
  },
  text: {
    color: 'white',
    fontSize: 18,
    fontWeight: 'bold',
    textAlign: 'center',
  },
});

export default ToggleButton;
