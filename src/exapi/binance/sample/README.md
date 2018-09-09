# Sample codes

## Using DAQuoteBinanceSpi + MyDAQuoteBinanceApi in async mode

1. Define an class (MyDAQuoteBinanceSpi) inherited from DAQuoteBinanceSpi:

    ```C++
    // see MyDAQuoteBinanceSpi.h
    ```

2. Initialize an API with **DAQuoteBinanceApi::CreateApi** and assign with an instance of above class:

    ```C++
    void test_binance_quote_spi()
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