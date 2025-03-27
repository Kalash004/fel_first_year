package connection_api.discovery;

/**
 * Accepts discovery requests and requests {@link connection_api.sending.Sender} to send
 * {@message_type <strong>conack</strong}
 */
public class DiscoveryServer {
}

/*
EXAMPLE SERVER STRUCTURE (source: https://cw.fel.cvut.cz/wiki/courses/b0b36pjv/tutorials/10/site)
Socket clientSocket = null;
ServerSocket serverSocket = null;

// ziskani portu nebo chyba
try {
  serverSocket = new ServerSocket(9903);
} catch (IOException e) {
    System.out.println("Port je uz obsazen.");
    System.exit(-1);
}

// ziskani bezneho socketu - bude se cekat do pripojeni klienta!
try {
    clientSocket = serverSocket.accept();
} catch (IOException e) {
    System.out.println("Pripojeni selhalo.");
    System.exit(-1);
}
 */