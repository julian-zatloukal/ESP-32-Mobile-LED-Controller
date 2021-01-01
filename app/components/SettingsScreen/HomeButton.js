import React from 'react';
import {StyleSheet, TouchableOpacity} from 'react-native';
import {FontAwesomeIcon} from '@fortawesome/react-native-fontawesome';
import {faHome} from '@fortawesome/free-solid-svg-icons';

const HomeButton = ({navigateToHome}) => {
  return (
    <TouchableOpacity
      style={styles.homeButtonContainer}
      onPress={navigateToHome}>
      <FontAwesomeIcon icon={faHome} size={32} style={styles.homeButton} />
    </TouchableOpacity>
  );
};

const styles = StyleSheet.create({
  homeButtonContainer: {
    margin: 0,
    marginTop: 1,
  },
  homeButton: {
    color: 'white',
  },
});

export default HomeButton;
