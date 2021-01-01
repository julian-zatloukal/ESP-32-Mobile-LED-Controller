import React, {useEffect} from 'react';
import {StatusBar} from 'react-native';

import {NavigationContainer} from '@react-navigation/native';
import {createStackNavigator} from '@react-navigation/stack';

import HomeScreen from './screens/HomeScreen';
import SettingsScreen from './screens/SettingsScreen';

import {SettingsProvider} from './Context/SettingsContext';
import {WebSocketProvider} from './Context/WebSocketContext';

const App = () => {
  useEffect(() => {
    StatusBar.setBarStyle('light-content', true);
    StatusBar.setBackgroundColor('#E80042');
  }, []);

  const Stack = createStackNavigator();

  return (
    <NavigationContainer>
      <Stack.Navigator
        initialRouteName="Home"
        screenOptions={{
          headerShown: false,
        }}>
        <Stack.Screen name="Home" component={HomeScreen} />
        <Stack.Screen name="Settings" component={SettingsScreen} />
      </Stack.Navigator>
    </NavigationContainer>
  );
};

export default () => (
  <SettingsProvider>
    <WebSocketProvider>
      <App />
    </WebSocketProvider>
  </SettingsProvider>
);
