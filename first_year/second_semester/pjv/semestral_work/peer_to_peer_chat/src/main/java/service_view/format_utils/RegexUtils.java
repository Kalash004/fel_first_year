package service_view.format_utils;

public class RegexUtils {
    public static boolean isAlphanumeric(String input) {
        return input.matches("^[a-zA-Z0-9]+$");
    }
}
