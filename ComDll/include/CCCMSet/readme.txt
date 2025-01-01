1、模块接口
MIDL_INTERFACE("xxxxx-xxxx-xxxx-xxxxxx")
iXXXXXKKKKK : IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE GetXXXXX(ICCMSet **pSet)
        = 0;
};

2、接口实现
class cXXXXKKKKKKK : iXXXXXKKKKK {}

3、接口实现使用
class zKKKKKK : ziXXXXXXX
{
	virtual HRESULT STDMETHODCALLTYPE GetXXXXX(ICCMSet **pSet)
        {
			CComObject<CCCMSet<cXXXXKKKKKKK>> data;
			....
			data.QueryInterface(pSet);
		}
}

4、接口调用
CCCMSetView<iXXXXXKKKKK> datas;
ppp->GetXXXXX(&datas);