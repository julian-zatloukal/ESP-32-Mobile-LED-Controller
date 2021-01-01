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
      handle: (values) => {
        console.log(
          `Handling set led bundle. Values ${JSON.stringify(values)}`,
        );

        if (values.length === 8) {
          const valuesInt = values.map((x) => Number.parseInt(x, 10));
          setLedArray(valuesInt);
        }
      },
      compose: (values) => {
        return `SET_LED_BUNDLE[${values.join('][')}]`;
      },
    },
  });

  const commandHandler = (textData) => {
    var commandCode = textData.substring(0, textData.indexOf('['));
    console.log(`Command code ${commandCode}`);
    Object.keys(commands.current).forEach((command) => {
      if (commands.current[command].code.includes(commandCode)) {
        var regExp = /\[([^[]+)\]/g;
        let paramsList = (textData.match(regExp) || []).map((e) =>
          e.replace(regExp, '$1'),
        );
        commands.current[command].handle(paramsList);
      }
    });
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
