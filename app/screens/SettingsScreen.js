import React, {useEffect} from 'react';
import {StyleSheet, View, StatusBar} from 'react-native';
import Header from '../components/SettingsScreen/Header';
import SettingModule from '../components/SettingsScreen/SettingModule';
import ConnectButton from '../components/SettingsScreen/ConnectButton';
import {useSettings} from '../Context/SettingsContext';

const SettingsScreen = ({route, navigation}) => {
  useEffect(() => {
    StatusBar.setBarStyle('light-content', true);
    StatusBar.setBackgroundColor('#E80042');
  }, []);

  const {getSettings, updateSettings} = useSettings();

  const [ipAddress, setIpAddress] = React.useState(getSettings().ipAddress);

  useEffect(() => {
    updateSettings({ipAddress: ipAddress});
  }, [updateSettings, ipAddress]);

  const navigateToHome = () => {
    navigation.navigate('Home');
  };

  return (
    <View style={styles.container}>
      <Header title="Settings" navigateToHome={navigateToHome} />
      <SettingModule
        name={'IP Address'}
        value={ipAddress}
        setValue={setIpAddress}
      />
      <ConnectButton ip={ipAddress} />
    </View>
  );
};
const styles = StyleSheet.create({
  container: {
    flex: 1,
    flexDirection: 'column',
    backgroundColor: '#FFFFFF',
  },
});

export default SettingsScreen;
