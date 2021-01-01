import React from 'react';
import {StyleSheet, View} from 'react-native';
import ToggleButton from '../components/HomeScreen/ToggleButton';

const LedMatrix = ({navigateToHome}) => {
  return (
    <View style={styles.matrixContainer}>
      <View style={styles.buttonMatrix}>
        {Array(8)
          .fill()
          .map((item, i) => (
            <ToggleButton index={i} key={i} />
          ))}
      </View>
    </View>
  );
};

const styles = StyleSheet.create({
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
});

export default LedMatrix;
