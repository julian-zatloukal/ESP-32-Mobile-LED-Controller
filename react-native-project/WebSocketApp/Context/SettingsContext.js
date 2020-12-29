import React, {useState, useEffect, useMemo} from 'react';

const SettingsContext = React.createContext();

export const SettingsProvider = (props) => {
  const [ipAddress, setIpAddress] = useState('192.168.4.1');
  const [port, setPort] = useState('8080');

  const updateSettings = (bundle) => {
    setIpAddress(bundle?.ipAddress);
    setPort(bundle?.port);
  };

  const getSettings = () => {
    return {ipAddress: ipAddress, port: port};
  };

  return (
    <SettingsContext.Provider
      value={{getSettings, updateSettings, ipAddress, port}}
      {...props}
    />
  );
};

export const useSettings = () => {
  const context = React.useContext(SettingsContext);
  if (!SettingsContext) {
    throw new Error('useSettings must be inside SettingsContext provider');
  }
  return context;
};
