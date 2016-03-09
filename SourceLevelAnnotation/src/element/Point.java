package element;

import annotations.ToElement;

@ToElement
public class Point {
    private int x;
    private int y;

    public Point(int x, int y) {
        this.x = x;
        this.y = y;
    }

    @ToElement
    public int getX() {
        return x;
    }

    @ToElement
    public int getY() {
        return y;
    }
}