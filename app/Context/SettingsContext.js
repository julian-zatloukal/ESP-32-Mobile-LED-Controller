import React, {useState, useEffect, useMemo} from 'react';

const SettingsContext = React.createContext();

export const SettingsProvider = (props) => {
  const [ipAddress, setIpAddress] = useState('192.168.4.1');

  const updateSettings = (bundle) => {
    setIpAddress(bundle?.ipAddress);
  };

  const getSettings = () => {
    return {ipAddress: ipAddress};
  };

  return (
    <SettingsContext.Provider
      value={{getSettings, updateSettings, ipAddress}}
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
