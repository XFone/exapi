# Sample codes

## Using OKCoinApiCom in sync mode

```C++
void test_okex_quote_sync()
{
    OKCoinApiCom api("", "");

    std::string symbol = "eth_btc";
    std::string json = api.GetTicker(symbol);

    cout << "json: " << json << endl;
}
```

## Using DAQuoteOkexApi + DAQuoteOkexSpi in async mode

1. Define an class (MyQuoteOkexSpi) inherited from DAQuoteOkexSpi:

    ```C++
    #include "quote/DAQuoteOkexApi.h"
    #include <iostream>

    using namespace std;

    class MyQuoteOkexSpi : public DAQuoteOkexSpi {
    public:
        MyQuoteOkexSpi() {
            cout << "** MyQuoteOkexSpi created" << endl;
        }

        virtual ~MyQuoteOkexSpi() {
            cout << "** MyQuoteOkexSpi deleted" << endl;
        }

        virtual void OnConnected(const char *addr) {
            cout << "** OnConnected" << endl;
        }

        virtual void OnDisconnected(const char *addr) {
            cout << "** OnDisconnected" << endl;
        }

        virtual void OnQuoteUpdated(int quoteType, void *pQuoteData) {
            cout << "** OnQuoteUpdated - type: "
                << quoteType << ", json:\n"
                << (const char *)pQuoteData << "\n"
                << endl;
        }

        virtual void OnSymbolSubscribed(int quoteType, const char *pSymbol, unsigned status) {
            cout << "** OnSymbolSubscribed" << endl;
        }

        virtual void OnSymbolUnsubscribed(int quoteType, const char *pSymbol, unsigned status) {
            cout << "** OnSymbolUnsubscribed" << endl;
        }
    };
    ```

2. Initialize an API with **DAQuoteOkexApi::CreateApi** and assign with an instance of above class:

    ```C++
    void test_okex_quote_spi()
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