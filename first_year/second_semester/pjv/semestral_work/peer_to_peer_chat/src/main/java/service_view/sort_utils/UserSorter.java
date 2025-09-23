package service_view.sort_utils;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;

public class UserSorter {
    private final ArrayList<String> sortOptions = new ArrayList<String>(Arrays.asList("Alphabetically", "Recently Chatted With", "Recently Connected"));

    public ArrayList<String> getSortOptions() {
        return sortOptions;
    }

    public static ArrayList<String> sortAlphabetically(ArrayList<String> users) {
        Collections.sort(users);
        return users;
    }

    public static ArrayList<String> sortRecentConnected(ArrayList<String> users) {
        return new ArrayList<>(); // TODO
    }

    public static ArrayList<String> sortRecentlyChattedWith(ArrayList<String> users) {
        return new ArrayList<>(); // TODO
    }
}
