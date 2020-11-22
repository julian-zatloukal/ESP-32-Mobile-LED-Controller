import React from 'react';
import {View, Text, StyleSheet, StatusBar, useEffect} from 'react-native';
import HomeButton from './HomeButton';

const Header = ({title, navigateToHome}) => {
  return (
    <View style={styles.header}>
      <Text style={styles.text}>{title}</Text>
      <View style={styles.saveAndHome}>
        <HomeButton navigateToHome={navigateToHome} />
      </View>
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
  },
  text: {
    color: 'white',
    fontSize: 23,
    alignSelf: 'center',
  },
  saveAndHome: {
    display: 'flex',
    flexDirection: 'row',
  },
});

export default Header;
