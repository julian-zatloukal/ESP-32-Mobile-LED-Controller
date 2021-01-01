import React from 'react';
import {
  View,
  Text,
  StyleSheet,
  TouchableOpacity,
  useEffect,
  TextInput,
} from 'react-native';

const SettingModule = ({name, value, setValue}) => {
  return (
    <View style={styles.container}>
      <Text
        style={{
          fontWeight: 'bold',
          fontSize: 14,
          marginBottom: 5,
          marginStart: 5,
          color: '#242424',
        }}>
        {name}
      </Text>
      <View style={[styles.textInputContainer, {borderColor: '#242424'}]}>
        <TextInput
          style={styles.textInput}
          onChangeText={(text) => setValue(text)}
          value={value}
        />
      </View>
    </View>
  );
};

const styles = StyleSheet.create({
  textInput: {
    height: 40,
    borderWidth: 0,
    color: '#242424',
  },
  textInputContainer: {
    borderRadius: 10,
    overflow: 'hidden',
    borderWidth: 1,
  },
  container: {
    marginHorizontal: 10,
    marginVertical: 10,
  },
});

export default SettingModule;
