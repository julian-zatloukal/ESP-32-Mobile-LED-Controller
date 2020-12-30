import React, {useState, useEffect, useRef} from 'react';

const WebSocketContext = React.createContext();

export const WebSocketProvider = (props) => {
  const ws = useRef(null);
  const [connectionStatus, setConnectionStatus] = useState(false);
  const connectionStatusRef = useRef(false);
  useEffect(() => {
    connectionStatusRef.current = connectionStatus;
  }, [connectionStatus]);

  const [ledArray, setLedArray] = useState([1, 1, 1, 1, 1, 1, 1, 1]);

  const initWebSocket = (ip, callback) => {
    console.log(`Attepting connection to ws://${ip}/`);
    ws.current = new WebSocket(`ws://${ip}/`);

    ws.current.onopen = () => {
      console.log('WebSocket Connected!');
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
      setConnectionStatus(false);
    }
  };

  useEffect(() => {
    let command = commands.current.setLedBundle.compose(ledArray);
    console.log(command);
    if (connectionStatusRef.current) {
      ws.current.send(command);
    }
  }, [ledArray]);

  const commands = useRef({
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
  });

  const commandHandler = (command) => {};

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
