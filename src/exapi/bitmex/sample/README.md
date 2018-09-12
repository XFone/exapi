# Sample codes

## Using DAQuoteBitmexSpi + MyDAQuoteBitmexApi in async mode

1. Define an class (MyDAQuoteBitmexSpi) inherited from DAQuoteBitmexSpi:

    ```C++
    // see MyDAQuoteBitmexSpi.h
    ```

2. Initialize an API with **DAQuoteBitmexApi::CreateApi** and assign with an instance of above class:

    ```C++
    void test_bitmex_quote_spi()
    {
        DAQuoteOkexApi *api = DAQuoteOkexApi::CreateApi();
        MyQuoteOkexSpi spi;

        api->ConnServer(nullptr, &spi);
        api->Init();

        api->GetTicker("etc_btc");

        api->Join();
        api->Dispose();
    }
    ```