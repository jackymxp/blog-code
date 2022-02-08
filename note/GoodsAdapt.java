package com.llw.cart.adapter;

import android.annotation.SuppressLint;
import android.text.Editable;
import android.text.InputType;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.Nullable;

import com.bumptech.glide.Glide;
import com.chad.library.adapter.base.BaseQuickAdapter;
import com.chad.library.adapter.base.BaseViewHolder;
import com.llw.cart.R;
import com.llw.cart.bean.OrderDataBean;

import java.util.List;

public class GoodsAdapter extends BaseQuickAdapter<OrderDataBean, BaseViewHolder> {

    private boolean isHidden = true;
    private static final String TAG = "GoodsAdapter";

    public GoodsAdapter(int layoutResId, @Nullable List<OrderDataBean> data) {
        super(layoutResId, data);
    }

    @SuppressLint("UseCompatLoadingForDrawables")
    @Override
    protected void convert(BaseViewHolder helper, OrderDataBean item) {
        helper.setIsRecyclable(false);//禁止复用

        TextView showType = helper.getView(R.id.show_type);
        showType.setText("礼物价值");
        EditText goodName = helper.getView(R.id.tv_good_name);
        EditText goodPrice = helper.getView(R.id.tv_goods_price);
        ImageView checkedGoods = helper.getView(R.id.iv_checked_goods);

        helper.setText(R.id.tv_good_name, item.getName())
                .setText(R.id.tv_goods_price, item.getPrice() + "");
        ImageView goodImg = helper.getView(R.id.iv_goods);
        Glide.with(mContext).load(item.getDefaultPic()).into(goodImg);

        //判断商品是否选中
        if (item.isChecked()) {
            checkedGoods.setImageDrawable(mContext.getDrawable(R.drawable.ic_checked));
        } else {
            checkedGoods.setImageDrawable(mContext.getDrawable(R.drawable.ic_check));
        }

        ImageView checkedBox = helper.getView(R.id.iv_checked_goods);

        if (!isHidden) { // 展示，可编辑状态
            checkedBox.setVisibility(View.VISIBLE);
            setEditTextEnabled(goodName, true);
            setEditTextEnabled(goodPrice, true);
        } else {
            checkedBox.setVisibility(View.GONE);
            setEditTextEnabled(goodName, false);
            setEditTextEnabled(goodPrice, false);
        }

        //添加点击事件
        helper.addOnClickListener(R.id.iv_checked_goods); //选中商品

        goodPrice.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void afterTextChanged(Editable editable) {
                String price = editable.toString();
                try {
                    item.setPrice(Integer.parseInt(price));
                } catch (Exception ignored) {

                }
            }
        });

        goodName.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {
            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
            }

            @Override
            public void afterTextChanged(Editable editable) {
                String name = editable.toString();
                item.setName(name);
            }
        });
    }

    @Override
    public int getItemViewType(int position) {
        return position;
    }

    @SuppressLint("NotifyDataSetChanged")
    public void addOrderDataBean(List<OrderDataBean> list) {
        mData.clear();
        mData = list;
        notifyDataSetChanged();
    }

    //  添加数据
    public void addOrderDataBean(int position, OrderDataBean o) {
        //在list中添加数据，并通知条目加入一条
        mData.add(position, o);
        //添加动画
        notifyItemInserted(position);
    }


    @SuppressLint("NotifyDataSetChanged")
    public void hiddenCheckBox(boolean isHidden) {
        if (isHidden != this.isHidden) {
            this.isHidden = isHidden;
            notifyDataSetChanged();
        }
    }

    private void setEditTextEnabled(EditText editText, boolean mode) {
        if (editText == null) {
            Log.d(TAG, "edittest is null");
            return;
        }
        editText.setFocusable(mode);
        editText.setFocusableInTouchMode(mode);
        editText.setLongClickable(mode);
        editText.setInputType(mode ? InputType.TYPE_CLASS_TEXT : InputType.TYPE_NULL);
    }

}
