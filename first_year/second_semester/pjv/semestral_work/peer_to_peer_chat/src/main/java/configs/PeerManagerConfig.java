package configs;

public class PeerManagerConfig {
    private static final int peerExpirationTimeMs = 10000; // 10s

    public static int getPeerExpirationTimeMs() {
        return peerExpirationTimeMs;
    }
}
