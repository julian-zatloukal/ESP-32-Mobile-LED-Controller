import React from 'react';
import {StyleSheet, TouchableOpacity} from 'react-native';
import {FontAwesomeIcon} from '@fortawesome/react-native-fontawesome';
import {faCogs} from '@fortawesome/free-solid-svg-icons';

const SettingsButton = ({navigateToSettings}) => {
  return (
    <TouchableOpacity
      style={styles.settingsButtonContainer}
      onPress={navigateToSettings}>
      <FontAwesomeIcon icon={faCogs} size={32} style={styles.settingsButton} />
    </TouchableOpacity>
  );
};

const styles = StyleSheet.create({
  settingsButtonContainer: {
    margin: 0,
    marginTop: 1,
  },
  settingsButton: {
    color: 'white',
  },
});

export default SettingsButton;
