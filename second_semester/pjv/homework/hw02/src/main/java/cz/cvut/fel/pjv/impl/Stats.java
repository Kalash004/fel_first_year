package cz.cvut.fel.pjv.impl;

import cz.cvut.fel.pjv.StatsInterface;

import java.util.ArrayList;

public class Stats implements StatsInterface {
    // TODO implement all methods from StatsInterface

    private final ArrayList<Double> list;
    private double sum;
    private int array_size;

    public Stats() {
        list = new ArrayList<Double>();
        sum = 0;
        array_size = 0;
    }

    @Override
    public void addNumber(double number) {
        list.add(number);
        sum += number;
        array_size += 1;
    }

    @Override
    public double getAverage() {
        return sum / array_size;
    }

    @Override
    public double getStandardDeviation() {
        double mean = getAverage();
        double dividend = 0f;

        for (double num : list) {
            dividend += Math.pow(num - mean, 2);
        }

        double divisor = array_size;
        if (divisor == 0) return 0f;
        return Math.sqrt(dividend / divisor);
    }

    @Override
    public int getCount() {
        return array_size;
    }

    @Override
    public String getFormattedStatistics() {
        return String.format("%.3f %.3f", getAverage(), getStandardDeviation());
    }

    public String getList() {
        return list.toString() + " " + array_size;
    }
}
