package connection_api.discovery;

import models.interfaces.IRunnable;

import java.io.*;
import java.net.*;
import java.util.*;

/**
 *  Sends discovery requests {@message_type <strong>hello</strong>}
 */
public class DiscoveryClient implements IRunnable {
    private String local_reader_ip;
    private int local_reader_port;
    private String target_ip_range;
    private String local_mask;
    private String target_range_mask;

    private Socket discoverySocket;

    public DiscoveryClient(Socket discoverySocket, String local_mask, String local_reader_ip, int local_reader_port, String target_ip_range, String target_range_mask) {
        this.discoverySocket = discoverySocket;
        this.local_mask = local_mask;
        this.local_reader_ip = local_reader_ip;
        this.local_reader_port = local_reader_port;
        this.target_ip_range = target_ip_range;
        this.target_range_mask = target_range_mask;
    }

    @Override
    public void run() {
        try {
            discoverySocket = new Socket();
        } catch (IOException e) {
            // TODO: Logger
            throw new RuntimeException(e);
        }
    }
}


// EXAMPLE CLIENT STRUCTURE (source: https://cw.fel.cvut.cz/wiki/courses/b0b36pjv/tutorials/10/site)


//public class EchoClient {
//    // cely klient je tak jednoduchy, ze je v metode main
//    public static void main(String[] args) {
//        // otevirani socketu
//
//        try {
//            clientSocket = new Socket("localhost", 9903);
//        }
//        catch (UnknownHostException ex) {
//            // Nepodarilo se najit (DNS, NIS atp.) hostitele
//            System.exit(-1);
//        }
//        catch (IOException ex) {
//            // Nepodarilo se spojit s hostitelem
//            System.exit(-1);
//        }
//        // parametry spojeni - vyprseni (pri cteni ze socketu)
//        try {
//            clientSocket.setSoTimeout(TIMEOUT_1M);
//        } catch (SocketException ex) {
//            // Nepodarilo se nastavit timeout
//        }
//        // otevreni proudu, spojeni
//        try {
//            PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
//            BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
//            BufferedReader systemIn = new BufferedReader(new InputStreamReader(System.in));
//
//            String input;
//            while ((input = systemIn.readLine()) != null) {
//                // takhle se posilaji data serveru
//                out.println(input);
//                // pokud mame skoncit, tak uzavreme otevrene proudy a socket
//                if (input.trim().equals("CLOSE")) {
//                    in.close();
//                    out.close();
//                    systemIn.close();
//                    clientSocket.close();
//                    break;
//                }
//                // jinak precteme, co server odpovedel a vypiseme
//                else {
//                    String response = in.readLine();
//                    System.out.println("RESPONSE: " + response);
//                }
//            }
//        }
//        catch (IOException ex) {
//            EchoLogger.log(ERROR,"Doslo k chybe I/O.");
//            System.exit(-1);
//        }
//    }
//}
