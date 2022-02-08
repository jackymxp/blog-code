package com.llw.cart.bean;



public class OrderDataBean {

    private static int incId;

    private int id;
    private String defaultPic;
    private String name;
    private int price;
    private boolean isChecked;//商品是否选中

    public OrderDataBean(String name, int price) {
        id = incId;
        this.price = price;
        this.name = name;
        incId++;
    }


    public boolean isChecked() {
        return isChecked;
    }

    public void setChecked(boolean checked) {
        isChecked = checked;
    }

    public String getDefaultPic() {
        return defaultPic;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getPrice() {
        return price;
    }

    public void setPrice(int price) {
        this.price = price;
    }


    @Override
    public String toString() {
        return "OrderDataBean{" +
                "id='" + id + '\'' +
                "defaultPic='" + defaultPic + '\'' +
                ", name='" + name + '\'' +
                ", price=" + price +
                ", isChecked=" + isChecked +
                '}';
    }
}


