package service_view;

import configs.UiConfig;

public class UiConfigurator {
    private static final UiConfigurator instance = new UiConfigurator();

    private UiConfigurator() {}

    public static UiConfigurator getInstance() {
        return instance;
    }

    public String getUiWindowName() {
        return UiConfig.getTitle();
    }
}
