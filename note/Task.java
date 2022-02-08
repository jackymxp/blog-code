package com.llw.cart;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.chad.library.adapter.base.BaseQuickAdapter;
import com.llw.cart.adapter.GoodsAdapter;
import com.llw.cart.bean.OrderDataBean;
import com.scwang.smartrefresh.layout.SmartRefreshLayout;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;

/**
 * 购物车
 *
 * @author llw
 */
public class GiftActivity extends AppCompatActivity implements View.OnClickListener {

    public static final String TAG = "MainActivity";

    private EditText goodName;
    private EditText goodPrice;
    private RecyclerView giftsView;
    private List<OrderDataBean> goodsList = new ArrayList<>();
    private TextView tvEdit;//编辑
    private LinearLayout layEdit;//编辑底部布局
    private TextView addGoods;//
    private TextView tvDeleteGoods;//删除商品

    private boolean isEditStatus = false;//是否编辑

    private AlertDialog dialog;//弹窗

    private OnClickItemListen startSecondActivityListen;

    private SmartRefreshLayout refresh;//刷新布局
    private LinearLayout layEmpty;//空布局
    GoodsAdapter goodsAdapter = new GoodsAdapter(R.layout.item_good, null);

    private static String[] PERMISSIONS_STORAGE = {
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.WRITE_EXTERNAL_STORAGE};
    private static final int REQUEST_PERMISSION_CODE = 1;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        getPermissions();

        initView();

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.d(TAG, "onDestroy");
        writeGoodsData(goodsList);
    }
    void getPermissions() {
        if (Build.VERSION.SDK_INT > Build.VERSION_CODES.LOLLIPOP) {
            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(this, PERMISSIONS_STORAGE, REQUEST_PERMISSION_CODE);
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == REQUEST_PERMISSION_CODE) {
            for (int i = 0; i < permissions.length; i++) {
                Log.i("MainActivity", "申请的权限为：" + permissions[i] + ",申请结果：" + grantResults[i]);
            }
        }
    }


    /**
     * 初始化
     */
    @SuppressLint({"NonConstantResourceId", "NotifyDataSetChanged"})
    private void initView() {
        //设置亮色状态栏模式 systemUiVisibility在Android11中弃用了，可以尝试一下。
        getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);

        giftsView = findViewById(R.id.rv_store);
        tvEdit = findViewById(R.id.tv_edit);
        layEdit = findViewById(R.id.lay_edit);
        addGoods = findViewById(R.id.add_good);
        tvDeleteGoods = findViewById(R.id.tv_delete_goods);
        refresh = findViewById(R.id.refresh);
        layEmpty = findViewById(R.id.lay_empty);
        goodName = findViewById(R.id.tv_good_name);
        goodPrice = findViewById(R.id.tv_goods_price);
        TextView titleLine = findViewById(R.id.title_line);
        titleLine.setText("奖励列表");

        //禁用下拉刷新和上拉加载更多
        refresh.setEnableRefresh(false);
        refresh.setEnableLoadMore(false);
        //下拉刷新监听
        //refresh.setOnRefreshListener(refreshLayout -> initView());


        tvEdit.setOnClickListener(this);
        addGoods.setOnClickListener(this);
        tvDeleteGoods.setOnClickListener(this);

        giftsView.setLayoutManager(new LinearLayoutManager(this));
        giftsView.setAdapter(goodsAdapter);


        // 设置监听事件
        startSecondActivityListen = new OnClickItemListen(this);
        goodsAdapter.setOnItemChildClickListener((adapter, view, position) -> {
            Log.d("TESTTT", "before  " + goodsList.toString());
            OrderDataBean goodsBean = goodsList.get(position);

            switch (view.getId()) {
                case R.id.iv_checked_goods://选中商品
                    goodsBean.setChecked(!goodsBean.isChecked());
                    break;
                default:
                    break;
            }
            goodsAdapter.notifyDataSetChanged();
            Log.d("TESTTT", "after  " + goodsList.toString());
        });
        goodsAdapter.setOnItemClickListener(startSecondActivityListen);

        //有商品
        //下拉加载数据完成后，关闭下拉刷新动画
        refresh.finishRefresh();
        //隐藏布局
        layEmpty.setVisibility(View.GONE);

        goodsList = getOrderDatas();

        goodsAdapter.addOrderDataBean(goodsList);
    }

    private List<OrderDataBean> getOrderDatas() {
        List<OrderDataBean> list = readResultBySerializable();;
        
        if(list == null)
            list = new ArrayList<>();
        return list;
    }

    @SuppressLint("SdCardPath")
    private final String giftFiles = "/data/data/com.llw.cart/cache/giftfiles";


    private List<OrderDataBean> readResultBySerializable() {
        List<OrderDataBean> listAsResult;
        ObjectInputStream ois = null;
        FileInputStream fis = null;
        try {
            if ((fis = new FileInputStream(giftFiles)) == null) {
                return null;
            }
            if ((ois = new ObjectInputStream(fis)) == null) {
                return null;
            }
            listAsResult = (ArrayList) ois.readObject();
        } catch (IOException | ClassNotFoundException ioe) {
            ioe.printStackTrace();
            return null;
        } finally {
            try {
                if (fis != null) {
                    fis.close();
                }
                if (ois != null) {
                    ois.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return listAsResult;
    }

    private void writeGoodsData(List<OrderDataBean> datas) {
        Log.d(TAG, "写入文件");
        Log.d(TAG, datas.toString());
        writeResultBySerializable(datas);
    }

    private void writeResultBySerializable(List<OrderDataBean> listAsResult) {
        if (listAsResult == null || listAsResult.isEmpty()) {
            Log.d(TAG, "writeResultBySerializable is null, return ");
            return;
        }
        ///data/data/com.llw.cart/cache
        Log.d(TAG, "writeResultBySerializable : " + listAsResult.toString());
        try {
            FileOutputStream fos = new FileOutputStream(new File(giftFiles));
            ObjectOutputStream oos = new ObjectOutputStream(fos);
            oos.writeObject(listAsResult);
            oos.close();
            fos.close();
        } catch (IOException ioe) {
            ioe.printStackTrace();
        }
    }


    static class OnClickItemListen implements BaseQuickAdapter.OnItemClickListener {
        private final Context mContext;

        public OnClickItemListen(Context context) {
            mContext = context;
        }

        @Override
        public void onItemClick(BaseQuickAdapter adapter, View view, int position) {
            Intent i = new Intent(mContext, TaskActivity.class);
            i.putExtra("position", position);
            mContext.startActivity(i);
        }
    }

    /**
     * 页面控件点击事件
     *
     * @param v
     */
    @SuppressLint({"NotifyDataSetChanged", "NonConstantResourceId"})
    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.tv_edit://编辑
//                if (goodsList.size() <= 0) {
//                    showMsg("当前购物车空空如也~");
//                    return;
//                }
                isEditStatus = !isEditStatus;
                if (!isEditStatus) {  // 当前处于不可编辑状态
                    tvEdit.setText("编辑");
                    layEdit.setVisibility(View.GONE);
                    goodsAdapter.hiddenCheckBox(true);
                    goodsAdapter.setOnItemClickListener(startSecondActivityListen);
                    hideKeyboard(this);  //隐藏输入键盘
                    // 将结果写入到文件中
                } else if (isEditStatus) {  // 编辑状态
                    tvEdit.setText("完成");
                    layEdit.setVisibility(View.VISIBLE);
                    goodsAdapter.hiddenCheckBox(false);
                    goodsAdapter.setOnItemClickListener(null);
                }
                break;

            case R.id.tv_delete_goods:
                if (getCheckBoxNumber() == 0) {
                    showMsg("请选择要删除的商品");
                    return;
                }
                //弹窗
                dialog = new AlertDialog.Builder(this)
                        .setMessage("确定要删除所选商品吗?")
                        .setNegativeButton("取消", (dialog, which) -> dialog.dismiss())
                        .setPositiveButton("确定", (dialog, which) -> deleteGoods())
                        .create();
                dialog.show();
                break;

            case R.id.add_good:
                layEmpty.setVisibility(View.GONE);
                showMsg("新增商品");
                OrderDataBean o = new OrderDataBean("input name", new Random().nextInt(100));
                goodsAdapter.addOrderDataBean(0, o);
                break;
            default:
                break;
        }
    }

    @SuppressLint("NotifyDataSetChanged")
    private void deleteGoods() {
        List<OrderDataBean> removeGoods = new ArrayList<>();
        for (OrderDataBean o : goodsList) {
            if (o.isChecked()) {
                Log.d(TAG, "delete: " + o);
                removeGoods.add(o);
            }
        }

        goodsList.removeAll(removeGoods);
        goodsAdapter.notifyDataSetChanged();
        if (goodsList.size() <= 0) {
            //启用下拉刷新
            refresh.setEnableRefresh(true);
            //显示空布局
            layEmpty.setVisibility(View.VISIBLE);
        }
    }


    private int getCheckBoxNumber() {
        int res = 0;
        for (OrderDataBean o : goodsList)
            if (o.isChecked())
                res++;
        return res;
    }

    /**
     * Toast提示
     *
     * @param msg
     */
    private void showMsg(String msg) {
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }

    public static void hideKeyboard(Activity context) {
        InputMethodManager imm = (InputMethodManager) context.getSystemService(Context.INPUT_METHOD_SERVICE);
        // 隐藏软键盘
        imm.hideSoftInputFromWindow(context.getWindow().getDecorView().getWindowToken(), 0);
    }
}
