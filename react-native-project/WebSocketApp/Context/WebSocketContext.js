import React, {useState, useEffect, useRef, useContext} from 'react';
import {ToastAndroid} from 'react-native';
import {useSettings} from './SettingsContext';

const WebSocketContext = React.createContext();

export const WebSocketProvider = (props) => {
  const ws = useRef(null);
  const autoConnectStatus = useRef(false);

  const [connectionStatus, setConnectionStatus] = useState(false);
  const settings = useSettings();

  const initWebSocket = (ip, port, callback) => {
    console.log(`Attepting connection to ws://${ip}:${port}/`);
    ws.current = new WebSocket(`ws://${ip}:${port}/`, 'echo-protocol');

    ws.current.onopen = () => {
      console.log('WebSocket Connected!');
      ws.current.send('Hello Server!');
      setConnectionStatus(true);
      callback(true);
    };
    ws.current.onclose = () => {
      console.log('WebSocket Disconnected!');
      setConnectionStatus(false);
      callback(false);
    };

    ws.current.onmessage = (event) => {
      console.log('Message received: ' + event.data);
      commandHandler(event.data);
    };

    return () => {
      ws.current.close();
    };
  };

  const closeWebSocket = () => {
    if (ws.current) {
      ws.current.close();
    }
  };

  const [ledArray, setLedArray] = useState([1, 1, 1, 1, 1, 1, 1, 1]);

  useEffect(() => {
    console.log(commands.setLedBundle.compose(ledArray));
    let command = commands.setLedBundle.compose(ledArray);
    //sendCommand(command);
    if (connectionStatus) {
      ws.current.send(command);
    }
  }, [ledArray, commands, connectionStatus]);

  const commands = {
    setLedBundle: {
      code: 'SET_LED_BUNDLE',
      handle: (values) => {},
      compose: (values) => {
        return `SET_LED_BUNDLE[${values.join('][')}]`;
      },
    },
    setLed: {
      code: 'SET_LED',
      handle: () => {},
      compose: (ledIndex) => {
        return `SET_LED[${ledIndex}]`;
      },
    },
    getLed: {
      code: 'GET_LED',
      handle: () => {},
      compose: () => {},
    },
    getLedBundle: {
      code: 'GET_LED_BUNDLE',
      handle: () => {},
      compose: () => {},
    },
  };

  const commandHandler = (command) => {};

  const sendCommand = (command) => {
    if (connectionStatus) {
      ws.current.send(command);
    }
  };

  return (
    <WebSocketContext.Provider
      value={{
        initWebSocket,
        connectionStatus,
        setConnectionStatus,
        ledArray,
        setLedArray,
        closeWebSocket,
      }}
      {...props}
    />
  );
};

export const useWebSockets = () => {
  const context = React.useContext(WebSocketContext);
  if (!WebSocketProvider) {
    throw new Error('useWebSockets must be inside WebSocketContext provider');
  }
  return context;
};
