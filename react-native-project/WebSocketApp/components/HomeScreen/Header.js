import React from 'react';
import {View, Text, StyleSheet, StatusBar, useEffect} from 'react-native';
import SettingsButton from './SettingsButton';

const Header = ({title, navigateToSettings}) => {
  return (
    <View style={styles.header}>
      <Text style={styles.text}>{title}</Text>
      <SettingsButton navigateToSettings={navigateToSettings} />
    </View>
  );
};

Header.defaultProps = {
  title: 'Blinking LED',
};

const styles = StyleSheet.create({
  header: {
    height: 60,
    padding: 15,
    backgroundColor: '#ff1053',
    display: 'flex',
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignContent: 'center',
  },
  text: {
    color: 'white',
    fontSize: 23,
    alignSelf: 'center',
  },
});

export default Header;
