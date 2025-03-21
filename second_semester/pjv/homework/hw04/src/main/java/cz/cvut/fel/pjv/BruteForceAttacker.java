package cz.cvut.fel.pjv;

public class BruteForceAttacker {
    // TODO
    public void breakPassword(int sizeOfPassword) {
        char[] chars = getCharacters();

    }

    private boolean recurive(String password) {
        if (tryOpen(password)) {
            return true;
        }
        
        return recurive(password);
    }
}
