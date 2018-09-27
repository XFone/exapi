# Sample codes

## 1. Using DAQuoteBitmexSpi in async mode

1. Define an class (MyDAQuoteBitmexSpi) inherited from DAQuoteBitmexSpi:

    see [MyDAQuoteBitmexSpi.h](./MyQuoteBitmexSpi.h)

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

## 2. Using DATraderBitmexSpi in async mode

