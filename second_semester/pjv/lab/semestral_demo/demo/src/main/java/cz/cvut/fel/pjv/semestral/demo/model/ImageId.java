package cz.cvut.fel.pjv.semestral.demo.model;

public enum ImageId {
    BACKGROUND("background.png"),
    BIRD("bird1.png"),
    TOP_PIPE("top.png"),
    BOTTOM_PIPE("bottom.png");

    private final String fileName;
    private double width;
    private double height;

    ImageId(String fileName) {
        this.fileName = fileName;
    }

    public String getFileName() {
        return fileName;
    }

    public double getWidth() {
        return width;
    }

    public void setWidth(double width) {
        this.width = width;
    }

    public double getHeight() {
        return height;
    }

    public void setHeight(double height) {
        this.height = height;
    }
}
