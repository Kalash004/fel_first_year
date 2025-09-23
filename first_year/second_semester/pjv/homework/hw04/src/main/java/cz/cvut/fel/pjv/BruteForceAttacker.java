package cz.cvut.fel.pjv;

import java.util.ArrayList;
import java.util.Collection;

public class BruteForceAttacker extends Thief {
    private int password_size;
    private int char_count;
    private char[] characters;

    public void breakPassword(int sizeOfPassword) {
        characters = getCharacters();
        password_size = sizeOfPassword;
        char_count = characters.length;
        ArrayList<Character> password = new ArrayList<>();
        for (int i = 0; i < password_size; i++) {
            password.add(characters[0]);
        }
        recursive(password, 0, 0);
    }

    private boolean recursive(ArrayList<Character> password, int pivotPoint, int char_id) {
        if (open(password)) {
            return true;
        }
        if (pivotPoint == (password_size - 1)) {
            int new_char_id = (char_id + 1);
            if (new_char_id > char_count - 1) {
                return false;
            }
            password.set(pivotPoint, characters[new_char_id]);
            ArrayList<Character> new_password = new ArrayList<>(password);
            return recursive(new_password, pivotPoint, new_char_id);
        }
        ArrayList<Character> new_password = new ArrayList<>(password);
        if (recursive(new_password, pivotPoint + 1, 0)) {
            return true;
        }
        int new_char_id = char_id + 1;
        if (new_char_id > char_count - 1) {
            return false;
        }
        password.set(pivotPoint, characters[new_char_id]);
        new_password = new ArrayList<>(password);
        return recursive(new_password, pivotPoint, new_char_id);
    }

    private boolean open(ArrayList<Character> password) {
        char[] passwrd_arr = new char[password_size];
        for (int i = 0; i < password_size; i++) {
            passwrd_arr[i] = password.get(i);
        }
        return tryOpen(passwrd_arr);
    }
}
