package service_view.format_utils;

import java.util.Calendar;
import java.util.Date;

public class DateUtils {
    public static String timestampToString(Date date) {
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(date);
        int minute = calendar.get(Calendar.MINUTE);
        int second = calendar.get(Calendar.SECOND);
        String dateStr = calendar.get(Calendar.HOUR_OF_DAY) + ":";
        if (minute < 10) {
            dateStr += "0" + minute;
        } else {
            dateStr += minute;
        }
        dateStr += ":";
        if (second < 10) {
            dateStr += "0" + second;
        } else {
            dateStr += second;
        }
        return dateStr;
    }
}
