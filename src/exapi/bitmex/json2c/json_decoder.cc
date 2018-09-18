/* $Id: 2018-09-19 02:01:12 $
 * 
 * This file is auto-generated by parser, DO NOT MODIFY! 
 * Copyright (c) 2014-2018 Zerone.IO. All rights reserved.
 * 
 */

#include "json_decoder.h"
#include "json2c.inc"

template<>
int from_json(JsonUtils::Json &json, Announcement &v)
{
    json.get("id", v.id);
    json.get("link", v.link);
    json.get("title", v.title);
    json.get("content", v.content);
    json.get_datetime("date", v.date);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, Error &v)
{
    json.get("error", v.error);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, APIKey &v)
{
    json.get("id", v.id);
    json.get("secret", v.secret);
    json.get("name", v.name);
    json.get("nonce", v.nonce);
    json.get("cidr", v.cidr);
    json.get("permissions", v.permissions);
    json.get("enabled", v.enabled);
    json.get("userId", v.userId);
    json.get_datetime("created", v.created);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, Chat &v)
{
    json.get("id", v.id);
    json.get_datetime("date", v.date);
    json.get("user", v.user);
    json.get("message", v.message);
    json.get("html", v.html);
    json.get("fromBot", v.fromBot);
    json.get("channelID", v.channelID);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, ChatChannel &v)
{
    json.get("id", v.id);
    json.get("name", v.name);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, ConnectedUsers &v)
{
    json.get("users", v.users);
    json.get("bots", v.bots);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, Execution &v)
{
    json.get("execID", v.execID);
    json.get("orderID", v.orderID);
    json.get("clOrdID", v.clOrdID);
    json.get("clOrdLinkID", v.clOrdLinkID);
    json.get("account", v.account);
    json.get("symbol", v.symbol);
    json.get("side", v.side);
    json.get("lastQty", v.lastQty);
    json.get("lastPx", v.lastPx);
    json.get("underlyingLastPx", v.underlyingLastPx);
    json.get("lastMkt", v.lastMkt);
    json.get("lastLiquidityInd", v.lastLiquidityInd);
    json.get("simpleOrderQty", v.simpleOrderQty);
    json.get("orderQty", v.orderQty);
    json.get("price", v.price);
    json.get("displayQty", v.displayQty);
    json.get("stopPx", v.stopPx);
    json.get("pegOffsetValue", v.pegOffsetValue);
    json.get("pegPriceType", v.pegPriceType);
    json.get("currency", v.currency);
    json.get("settlCurrency", v.settlCurrency);
    json.get("execType", v.execType);
    json.get("ordType", v.ordType);
    json.get("timeInForce", v.timeInForce);
    json.get("execInst", v.execInst);
    json.get("contingencyType", v.contingencyType);
    json.get("exDestination", v.exDestination);
    json.get("ordStatus", v.ordStatus);
    json.get("triggered", v.triggered);
    json.get("workingIndicator", v.workingIndicator);
    json.get("ordRejReason", v.ordRejReason);
    json.get("simpleLeavesQty", v.simpleLeavesQty);
    json.get("leavesQty", v.leavesQty);
    json.get("simpleCumQty", v.simpleCumQty);
    json.get("cumQty", v.cumQty);
    json.get("avgPx", v.avgPx);
    json.get("commission", v.commission);
    json.get("tradePublishIndicator", v.tradePublishIndicator);
    json.get("multiLegReportingType", v.multiLegReportingType);
    json.get("text", v.text);
    json.get("trdMatchID", v.trdMatchID);
    json.get("execCost", v.execCost);
    json.get("execComm", v.execComm);
    json.get("homeNotional", v.homeNotional);
    json.get("foreignNotional", v.foreignNotional);
    json.get_datetime("transactTime", v.transactTime);
    json.get_datetime("timestamp", v.timestamp);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, Funding &v)
{
    json.get_datetime("timestamp", v.timestamp);
    json.get("symbol", v.symbol);
    json.get_datetime("fundingInterval", v.fundingInterval);
    json.get("fundingRate", v.fundingRate);
    json.get("fundingRateDaily", v.fundingRateDaily);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, Instrument &v)
{
    json.get("symbol", v.symbol);
    json.get("rootSymbol", v.rootSymbol);
    json.get("state", v.state);
    json.get("typ", v.typ);
    json.get_datetime("listing", v.listing);
    json.get_datetime("front", v.front);
    json.get_datetime("expiry", v.expiry);
    json.get_datetime("settle", v.settle);
    json.get_datetime("relistInterval", v.relistInterval);
    json.get("inverseLeg", v.inverseLeg);
    json.get("sellLeg", v.sellLeg);
    json.get("buyLeg", v.buyLeg);
    json.get("optionStrikePcnt", v.optionStrikePcnt);
    json.get("optionStrikeRound", v.optionStrikeRound);
    json.get("optionStrikePrice", v.optionStrikePrice);
    json.get("optionMultiplier", v.optionMultiplier);
    json.get("positionCurrency", v.positionCurrency);
    json.get("underlying", v.underlying);
    json.get("quoteCurrency", v.quoteCurrency);
    json.get("underlyingSymbol", v.underlyingSymbol);
    json.get("reference", v.reference);
    json.get("referenceSymbol", v.referenceSymbol);
    json.get_datetime("calcInterval", v.calcInterval);
    json.get_datetime("publishInterval", v.publishInterval);
    json.get_datetime("publishTime", v.publishTime);
    json.get("maxOrderQty", v.maxOrderQty);
    json.get("maxPrice", v.maxPrice);
    json.get("lotSize", v.lotSize);
    json.get("tickSize", v.tickSize);
    json.get("multiplier", v.multiplier);
    json.get("settlCurrency", v.settlCurrency);
    json.get("underlyingToPositionMultiplier", v.underlyingToPositionMultiplier);
    json.get("underlyingToSettleMultiplier", v.underlyingToSettleMultiplier);
    json.get("quoteToSettleMultiplier", v.quoteToSettleMultiplier);
    json.get("isQuanto", v.isQuanto);
    json.get("isInverse", v.isInverse);
    json.get("initMargin", v.initMargin);
    json.get("maintMargin", v.maintMargin);
    json.get("riskLimit", v.riskLimit);
    json.get("riskStep", v.riskStep);
    json.get("limit", v.limit);
    json.get("capped", v.capped);
    json.get("taxed", v.taxed);
    json.get("deleverage", v.deleverage);
    json.get("makerFee", v.makerFee);
    json.get("takerFee", v.takerFee);
    json.get("settlementFee", v.settlementFee);
    json.get("insuranceFee", v.insuranceFee);
    json.get("fundingBaseSymbol", v.fundingBaseSymbol);
    json.get("fundingQuoteSymbol", v.fundingQuoteSymbol);
    json.get("fundingPremiumSymbol", v.fundingPremiumSymbol);
    json.get_datetime("fundingTimestamp", v.fundingTimestamp);
    json.get_datetime("fundingInterval", v.fundingInterval);
    json.get("fundingRate", v.fundingRate);
    json.get("indicativeFundingRate", v.indicativeFundingRate);
    json.get_datetime("rebalanceTimestamp", v.rebalanceTimestamp);
    json.get_datetime("rebalanceInterval", v.rebalanceInterval);
    json.get_datetime("openingTimestamp", v.openingTimestamp);
    json.get_datetime("closingTimestamp", v.closingTimestamp);
    json.get_datetime("sessionInterval", v.sessionInterval);
    json.get("prevClosePrice", v.prevClosePrice);
    json.get("limitDownPrice", v.limitDownPrice);
    json.get("limitUpPrice", v.limitUpPrice);
    json.get("bankruptLimitDownPrice", v.bankruptLimitDownPrice);
    json.get("bankruptLimitUpPrice", v.bankruptLimitUpPrice);
    json.get("prevTotalVolume", v.prevTotalVolume);
    json.get("totalVolume", v.totalVolume);
    json.get("volume", v.volume);
    json.get("volume24h", v.volume24h);
    json.get("prevTotalTurnover", v.prevTotalTurnover);
    json.get("totalTurnover", v.totalTurnover);
    json.get("turnover", v.turnover);
    json.get("turnover24h", v.turnover24h);
    json.get("prevPrice24h", v.prevPrice24h);
    json.get("vwap", v.vwap);
    json.get("highPrice", v.highPrice);
    json.get("lowPrice", v.lowPrice);
    json.get("lastPrice", v.lastPrice);
    json.get("lastPriceProtected", v.lastPriceProtected);
    json.get("lastTickDirection", v.lastTickDirection);
    json.get("lastChangePcnt", v.lastChangePcnt);
    json.get("bidPrice", v.bidPrice);
    json.get("midPrice", v.midPrice);
    json.get("askPrice", v.askPrice);
    json.get("impactBidPrice", v.impactBidPrice);
    json.get("impactMidPrice", v.impactMidPrice);
    json.get("impactAskPrice", v.impactAskPrice);
    json.get("hasLiquidity", v.hasLiquidity);
    json.get("openInterest", v.openInterest);
    json.get("openValue", v.openValue);
    json.get("fairMethod", v.fairMethod);
    json.get("fairBasisRate", v.fairBasisRate);
    json.get("fairBasis", v.fairBasis);
    json.get("fairPrice", v.fairPrice);
    json.get("markMethod", v.markMethod);
    json.get("markPrice", v.markPrice);
    json.get("indicativeTaxRate", v.indicativeTaxRate);
    json.get("indicativeSettlePrice", v.indicativeSettlePrice);
    json.get("optionUnderlyingPrice", v.optionUnderlyingPrice);
    json.get("settledPrice", v.settledPrice);
    json.get_datetime("timestamp", v.timestamp);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, InstrumentInterval &v)
{
    json.get("intervals", v.intervals);
    json.get("symbols", v.symbols);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, IndexComposite &v)
{
    json.get_datetime("timestamp", v.timestamp);
    json.get("symbol", v.symbol);
    json.get("indexSymbol", v.indexSymbol);
    json.get("reference", v.reference);
    json.get("lastPrice", v.lastPrice);
    json.get("weight", v.weight);
    json.get_datetime("logged", v.logged);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, Insurance &v)
{
    json.get("currency", v.currency);
    json.get_datetime("timestamp", v.timestamp);
    json.get("walletBalance", v.walletBalance);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, Leaderboard &v)
{
    json.get("name", v.name);
    json.get("isRealName", v.isRealName);
    json.get("profit", v.profit);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, Liquidation &v)
{
    json.get("orderID", v.orderID);
    json.get("symbol", v.symbol);
    json.get("side", v.side);
    json.get("price", v.price);
    json.get("leavesQty", v.leavesQty);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, Notification &v)
{
    json.get("id", v.id);
    json.get_datetime("date", v.date);
    json.get("title", v.title);
    json.get("body", v.body);
    json.get("ttl", v.ttl);
    json.get("type", v.type);
    json.get("closable", v.closable);
    json.get("persist", v.persist);
    json.get("waitForVisibility", v.waitForVisibility);
    json.get("sound", v.sound);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, Order &v)
{
    json.get("orderID", v.orderID);
    json.get("clOrdID", v.clOrdID);
    json.get("clOrdLinkID", v.clOrdLinkID);
    json.get("account", v.account);
    json.get("symbol", v.symbol);
    json.get("side", v.side);
    json.get("simpleOrderQty", v.simpleOrderQty);
    json.get("orderQty", v.orderQty);
    json.get("price", v.price);
    json.get("displayQty", v.displayQty);
    json.get("stopPx", v.stopPx);
    json.get("pegOffsetValue", v.pegOffsetValue);
    json.get("pegPriceType", v.pegPriceType);
    json.get("currency", v.currency);
    json.get("settlCurrency", v.settlCurrency);
    json.get("ordType", v.ordType);
    json.get("timeInForce", v.timeInForce);
    json.get("execInst", v.execInst);
    json.get("contingencyType", v.contingencyType);
    json.get("exDestination", v.exDestination);
    json.get("ordStatus", v.ordStatus);
    json.get("triggered", v.triggered);
    json.get("workingIndicator", v.workingIndicator);
    json.get("ordRejReason", v.ordRejReason);
    json.get("simpleLeavesQty", v.simpleLeavesQty);
    json.get("leavesQty", v.leavesQty);
    json.get("simpleCumQty", v.simpleCumQty);
    json.get("cumQty", v.cumQty);
    json.get("avgPx", v.avgPx);
    json.get("multiLegReportingType", v.multiLegReportingType);
    json.get("text", v.text);
    json.get_datetime("transactTime", v.transactTime);
    json.get_datetime("timestamp", v.timestamp);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, OrderBookL2 &v)
{
    json.get("symbol", v.symbol);
    json.get("id", v.id);
    json.get("side", v.side);
    json.get("size", v.size);
    json.get("price", v.price);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, Position &v)
{
    json.get("account", v.account);
    json.get("symbol", v.symbol);
    json.get("currency", v.currency);
    json.get("underlying", v.underlying);
    json.get("quoteCurrency", v.quoteCurrency);
    json.get("commission", v.commission);
    json.get("initMarginReq", v.initMarginReq);
    json.get("maintMarginReq", v.maintMarginReq);
    json.get("riskLimit", v.riskLimit);
    json.get("leverage", v.leverage);
    json.get("crossMargin", v.crossMargin);
    json.get("deleveragePercentile", v.deleveragePercentile);
    json.get("rebalancedPnl", v.rebalancedPnl);
    json.get("prevRealisedPnl", v.prevRealisedPnl);
    json.get("prevUnrealisedPnl", v.prevUnrealisedPnl);
    json.get("prevClosePrice", v.prevClosePrice);
    json.get_datetime("openingTimestamp", v.openingTimestamp);
    json.get("openingQty", v.openingQty);
    json.get("openingCost", v.openingCost);
    json.get("openingComm", v.openingComm);
    json.get("openOrderBuyQty", v.openOrderBuyQty);
    json.get("openOrderBuyCost", v.openOrderBuyCost);
    json.get("openOrderBuyPremium", v.openOrderBuyPremium);
    json.get("openOrderSellQty", v.openOrderSellQty);
    json.get("openOrderSellCost", v.openOrderSellCost);
    json.get("openOrderSellPremium", v.openOrderSellPremium);
    json.get("execBuyQty", v.execBuyQty);
    json.get("execBuyCost", v.execBuyCost);
    json.get("execSellQty", v.execSellQty);
    json.get("execSellCost", v.execSellCost);
    json.get("execQty", v.execQty);
    json.get("execCost", v.execCost);
    json.get("execComm", v.execComm);
    json.get_datetime("currentTimestamp", v.currentTimestamp);
    json.get("currentQty", v.currentQty);
    json.get("currentCost", v.currentCost);
    json.get("currentComm", v.currentComm);
    json.get("realisedCost", v.realisedCost);
    json.get("unrealisedCost", v.unrealisedCost);
    json.get("grossOpenCost", v.grossOpenCost);
    json.get("grossOpenPremium", v.grossOpenPremium);
    json.get("grossExecCost", v.grossExecCost);
    json.get("isOpen", v.isOpen);
    json.get("markPrice", v.markPrice);
    json.get("markValue", v.markValue);
    json.get("riskValue", v.riskValue);
    json.get("homeNotional", v.homeNotional);
    json.get("foreignNotional", v.foreignNotional);
    json.get("posState", v.posState);
    json.get("posCost", v.posCost);
    json.get("posCost2", v.posCost2);
    json.get("posCross", v.posCross);
    json.get("posInit", v.posInit);
    json.get("posComm", v.posComm);
    json.get("posLoss", v.posLoss);
    json.get("posMargin", v.posMargin);
    json.get("posMaint", v.posMaint);
    json.get("posAllowance", v.posAllowance);
    json.get("taxableMargin", v.taxableMargin);
    json.get("initMargin", v.initMargin);
    json.get("maintMargin", v.maintMargin);
    json.get("sessionMargin", v.sessionMargin);
    json.get("targetExcessMargin", v.targetExcessMargin);
    json.get("varMargin", v.varMargin);
    json.get("realisedGrossPnl", v.realisedGrossPnl);
    json.get("realisedTax", v.realisedTax);
    json.get("realisedPnl", v.realisedPnl);
    json.get("unrealisedGrossPnl", v.unrealisedGrossPnl);
    json.get("longBankrupt", v.longBankrupt);
    json.get("shortBankrupt", v.shortBankrupt);
    json.get("taxBase", v.taxBase);
    json.get("indicativeTaxRate", v.indicativeTaxRate);
    json.get("indicativeTax", v.indicativeTax);
    json.get("unrealisedTax", v.unrealisedTax);
    json.get("unrealisedPnl", v.unrealisedPnl);
    json.get("unrealisedPnlPcnt", v.unrealisedPnlPcnt);
    json.get("unrealisedRoePcnt", v.unrealisedRoePcnt);
    json.get("simpleQty", v.simpleQty);
    json.get("simpleCost", v.simpleCost);
    json.get("simpleValue", v.simpleValue);
    json.get("simplePnl", v.simplePnl);
    json.get("simplePnlPcnt", v.simplePnlPcnt);
    json.get("avgCostPrice", v.avgCostPrice);
    json.get("avgEntryPrice", v.avgEntryPrice);
    json.get("breakEvenPrice", v.breakEvenPrice);
    json.get("marginCallPrice", v.marginCallPrice);
    json.get("liquidationPrice", v.liquidationPrice);
    json.get("bankruptPrice", v.bankruptPrice);
    json.get_datetime("timestamp", v.timestamp);
    json.get("lastPrice", v.lastPrice);
    json.get("lastValue", v.lastValue);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, Quote &v)
{
    json.get_datetime("timestamp", v.timestamp);
    json.get("symbol", v.symbol);
    json.get("bidSize", v.bidSize);
    json.get("bidPrice", v.bidPrice);
    json.get("askPrice", v.askPrice);
    json.get("askSize", v.askSize);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, Settlement &v)
{
    json.get_datetime("timestamp", v.timestamp);
    json.get("symbol", v.symbol);
    json.get("settlementType", v.settlementType);
    json.get("settledPrice", v.settledPrice);
    json.get("optionStrikePrice", v.optionStrikePrice);
    json.get("optionUnderlyingPrice", v.optionUnderlyingPrice);
    json.get("bankrupt", v.bankrupt);
    json.get("taxBase", v.taxBase);
    json.get("taxRate", v.taxRate);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, Stats &v)
{
    json.get("rootSymbol", v.rootSymbol);
    json.get("currency", v.currency);
    json.get("volume24h", v.volume24h);
    json.get("turnover24h", v.turnover24h);
    json.get("openInterest", v.openInterest);
    json.get("openValue", v.openValue);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, StatsHistory &v)
{
    json.get_datetime("date", v.date);
    json.get("rootSymbol", v.rootSymbol);
    json.get("currency", v.currency);
    json.get("volume", v.volume);
    json.get("turnover", v.turnover);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, StatsUSD &v)
{
    json.get("rootSymbol", v.rootSymbol);
    json.get("currency", v.currency);
    json.get("turnover24h", v.turnover24h);
    json.get("turnover30d", v.turnover30d);
    json.get("turnover365d", v.turnover365d);
    json.get("turnover", v.turnover);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, Trade &v)
{
    json.get_datetime("timestamp", v.timestamp);
    json.get("symbol", v.symbol);
    json.get("side", v.side);
    json.get("size", v.size);
    json.get("price", v.price);
    json.get("tickDirection", v.tickDirection);
    json.get("trdMatchID", v.trdMatchID);
    json.get("grossValue", v.grossValue);
    json.get("homeNotional", v.homeNotional);
    json.get("foreignNotional", v.foreignNotional);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, TradeBin &v)
{
    json.get_datetime("timestamp", v.timestamp);
    json.get("symbol", v.symbol);
    json.get("open", v.open);
    json.get("high", v.high);
    json.get("low", v.low);
    json.get("close", v.close);
    json.get("trades", v.trades);
    json.get("volume", v.volume);
    json.get("vwap", v.vwap);
    json.get("lastSize", v.lastSize);
    json.get("turnover", v.turnover);
    json.get("homeNotional", v.homeNotional);
    json.get("foreignNotional", v.foreignNotional);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, Wallet &v)
{
    json.get("account", v.account);
    json.get("currency", v.currency);
    json.get("prevDeposited", v.prevDeposited);
    json.get("prevWithdrawn", v.prevWithdrawn);
    json.get("prevTransferIn", v.prevTransferIn);
    json.get("prevTransferOut", v.prevTransferOut);
    json.get("prevAmount", v.prevAmount);
    json.get_datetime("prevTimestamp", v.prevTimestamp);
    json.get("deltaDeposited", v.deltaDeposited);
    json.get("deltaWithdrawn", v.deltaWithdrawn);
    json.get("deltaTransferIn", v.deltaTransferIn);
    json.get("deltaTransferOut", v.deltaTransferOut);
    json.get("deltaAmount", v.deltaAmount);
    json.get("deposited", v.deposited);
    json.get("withdrawn", v.withdrawn);
    json.get("transferIn", v.transferIn);
    json.get("transferOut", v.transferOut);
    json.get("amount", v.amount);
    json.get("pendingCredit", v.pendingCredit);
    json.get("pendingDebit", v.pendingDebit);
    json.get("confirmedDebit", v.confirmedDebit);
    json.get_datetime("timestamp", v.timestamp);
    json.get("addr", v.addr);
    json.get("script", v.script);
    json.get("withdrawalLock", v.withdrawalLock);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, Transaction &v)
{
    json.get("transactID", v.transactID);
    json.get("account", v.account);
    json.get("currency", v.currency);
    json.get("transactType", v.transactType);
    json.get("amount", v.amount);
    json.get("fee", v.fee);
    json.get("transactStatus", v.transactStatus);
    json.get("address", v.address);
    json.get("tx", v.tx);
    json.get("text", v.text);
    json.get_datetime("transactTime", v.transactTime);
    json.get_datetime("timestamp", v.timestamp);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, AccessToken &v)
{
    json.get("id", v.id);
    json.get("ttl", v.ttl);
    json.get_datetime("created", v.created);
    json.get("userId", v.userId);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, Affiliate &v)
{
    json.get("account", v.account);
    json.get("currency", v.currency);
    json.get("prevPayout", v.prevPayout);
    json.get("prevTurnover", v.prevTurnover);
    json.get("prevComm", v.prevComm);
    json.get_datetime("prevTimestamp", v.prevTimestamp);
    json.get("execTurnover", v.execTurnover);
    json.get("execComm", v.execComm);
    json.get("totalReferrals", v.totalReferrals);
    json.get("totalTurnover", v.totalTurnover);
    json.get("totalComm", v.totalComm);
    json.get("payoutPcnt", v.payoutPcnt);
    json.get("pendingPayout", v.pendingPayout);
    json.get_datetime("timestamp", v.timestamp);
    json.get("referrerAccount", v.referrerAccount);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, UserPreferences &v)
{
    json.get("alertOnLiquidations", v.alertOnLiquidations);
    json.get("animationsEnabled", v.animationsEnabled);
    json.get_datetime("announcementsLastSeen", v.announcementsLastSeen);
    json.get("chatChannelID", v.chatChannelID);
    json.get("colorTheme", v.colorTheme);
    json.get("currency", v.currency);
    json.get("debug", v.debug);
    json.get("disableEmails", v.disableEmails);
    json.get("hideConfirmDialogs", v.hideConfirmDialogs);
    json.get("hideConnectionModal", v.hideConnectionModal);
    json.get("hideFromLeaderboard", v.hideFromLeaderboard);
    json.get("hideNameFromLeaderboard", v.hideNameFromLeaderboard);
    json.get("hideNotifications", v.hideNotifications);
    json.get("locale", v.locale);
    json.get("msgsSeen", v.msgsSeen);
    json.get("orderBookBinning", v.orderBookBinning);
    json.get("orderBookType", v.orderBookType);
    json.get("orderClearImmediate", v.orderClearImmediate);
    json.get("orderControlsPlusMinus", v.orderControlsPlusMinus);
    json.get("showLocaleNumbers", v.showLocaleNumbers);
    json.get("sounds", v.sounds);
    json.get("strictIPCheck", v.strictIPCheck);
    json.get("strictTimeout", v.strictTimeout);
    json.get("tickerGroup", v.tickerGroup);
    json.get("tickerPinned", v.tickerPinned);
    json.get("tradeLayout", v.tradeLayout);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, User &v)
{
    json.get("id", v.id);
    json.get("ownerId", v.ownerId);
    json.get("firstname", v.firstname);
    json.get("lastname", v.lastname);
    json.get("username", v.username);
    json.get("email", v.email);
    json.get("phone", v.phone);
    json.get_datetime("created", v.created);
    json.get_datetime("lastUpdated", v.lastUpdated);
    //json.get("preferences", v.preferences);
    //from_json(json.get("preferences"), v.preferences);
    json.get("TFAEnabled", v.TFAEnabled);
    json.get("affiliateID", v.affiliateID);
    json.get("pgpPubKey", v.pgpPubKey);
    json.get("country", v.country);
    json.get("geoipCountry", v.geoipCountry);
    json.get("geoipRegion", v.geoipRegion);
    json.get("typ", v.typ);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, UserCommission &v)
{
    json.get("makerFee", v.makerFee);
    json.get("takerFee", v.takerFee);
    json.get("settlementFee", v.settlementFee);
    json.get("maxFee", v.maxFee);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, Margin &v)
{
    json.get("account", v.account);
    json.get("currency", v.currency);
    json.get("riskLimit", v.riskLimit);
    json.get("prevState", v.prevState);
    json.get("state", v.state);
    json.get("action", v.action);
    json.get("amount", v.amount);
    json.get("pendingCredit", v.pendingCredit);
    json.get("pendingDebit", v.pendingDebit);
    json.get("confirmedDebit", v.confirmedDebit);
    json.get("prevRealisedPnl", v.prevRealisedPnl);
    json.get("prevUnrealisedPnl", v.prevUnrealisedPnl);
    json.get("grossComm", v.grossComm);
    json.get("grossOpenCost", v.grossOpenCost);
    json.get("grossOpenPremium", v.grossOpenPremium);
    json.get("grossExecCost", v.grossExecCost);
    json.get("grossMarkValue", v.grossMarkValue);
    json.get("riskValue", v.riskValue);
    json.get("taxableMargin", v.taxableMargin);
    json.get("initMargin", v.initMargin);
    json.get("maintMargin", v.maintMargin);
    json.get("sessionMargin", v.sessionMargin);
    json.get("targetExcessMargin", v.targetExcessMargin);
    json.get("varMargin", v.varMargin);
    json.get("realisedPnl", v.realisedPnl);
    json.get("unrealisedPnl", v.unrealisedPnl);
    json.get("indicativeTax", v.indicativeTax);
    json.get("unrealisedProfit", v.unrealisedProfit);
    json.get("syntheticMargin", v.syntheticMargin);
    json.get("walletBalance", v.walletBalance);
    json.get("marginBalance", v.marginBalance);
    json.get("marginBalancePcnt", v.marginBalancePcnt);
    json.get("marginLeverage", v.marginLeverage);
    json.get("marginUsedPcnt", v.marginUsedPcnt);
    json.get("excessMargin", v.excessMargin);
    json.get("excessMarginPcnt", v.excessMarginPcnt);
    json.get("availableMargin", v.availableMargin);
    json.get("withdrawableMargin", v.withdrawableMargin);
    json.get_datetime("timestamp", v.timestamp);
    json.get("grossLastValue", v.grossLastValue);
    json.get("commission", v.commission);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, AnnouncementGetParams &v)
{
    json.get("columns", v.columns);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, APIKeyNewParams &v)
{
    json.get("name", v.name);
    json.get("cidr", v.cidr);
    json.get("permissions", v.permissions);
    json.get("enabled", v.enabled);
    json.get("token", v.token);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, APIKeyDisableParams &v)
{
    json.get("apiKeyID", v.apiKeyID);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, APIKeyEnableParams &v)
{
    json.get("apiKeyID", v.apiKeyID);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, ChatGetParams &v)
{
    json.get("count", v.count);
    json.get("start", v.start);
    json.get("reverse", v.reverse);
    json.get("channelID", v.channelID);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, ExecutionGetParams &v)
{
    json.get("symbol", v.symbol);
    json.get("filter", v.filter);
    json.get("columns", v.columns);
    json.get("count", v.count);
    json.get("start", v.start);
    json.get("reverse", v.reverse);
    json.get_datetime("startTime", v.startTime);
    json.get_datetime("endTime", v.endTime);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, ExecutionGetTradeHistoryParams &v)
{
    json.get("symbol", v.symbol);
    json.get("filter", v.filter);
    json.get("columns", v.columns);
    json.get("count", v.count);
    json.get("start", v.start);
    json.get("reverse", v.reverse);
    json.get_datetime("startTime", v.startTime);
    json.get_datetime("endTime", v.endTime);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, FundingGetParams &v)
{
    json.get("symbol", v.symbol);
    json.get("filter", v.filter);
    json.get("columns", v.columns);
    json.get("count", v.count);
    json.get("start", v.start);
    json.get("reverse", v.reverse);
    json.get_datetime("startTime", v.startTime);
    json.get_datetime("endTime", v.endTime);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, InstrumentGetParams &v)
{
    json.get("symbol", v.symbol);
    json.get("filter", v.filter);
    json.get("columns", v.columns);
    json.get("count", v.count);
    json.get("start", v.start);
    json.get("reverse", v.reverse);
    json.get_datetime("startTime", v.startTime);
    json.get_datetime("endTime", v.endTime);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, InstrumentGetCompositeIndexParams &v)
{
    json.get("account", v.account);
    json.get("symbol", v.symbol);
    json.get("filter", v.filter);
    json.get("columns", v.columns);
    json.get("count", v.count);
    json.get("start", v.start);
    json.get("reverse", v.reverse);
    json.get_datetime("startTime", v.startTime);
    json.get_datetime("endTime", v.endTime);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, InsuranceGetParams &v)
{
    json.get("symbol", v.symbol);
    json.get("filter", v.filter);
    json.get("columns", v.columns);
    json.get("count", v.count);
    json.get("start", v.start);
    json.get("reverse", v.reverse);
    json.get_datetime("startTime", v.startTime);
    json.get_datetime("endTime", v.endTime);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, LeaderboardGetParams &v)
{
    json.get("method", v.method);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, LiquidationGetParams &v)
{
    json.get("symbol", v.symbol);
    json.get("filter", v.filter);
    json.get("columns", v.columns);
    json.get("count", v.count);
    json.get("start", v.start);
    json.get("reverse", v.reverse);
    json.get_datetime("startTime", v.startTime);
    json.get_datetime("endTime", v.endTime);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, OrderGetOrdersParams &v)
{
    json.get("symbol", v.symbol);
    json.get("filter", v.filter);
    json.get("columns", v.columns);
    json.get("count", v.count);
    json.get("start", v.start);
    json.get("reverse", v.reverse);
    json.get_datetime("startTime", v.startTime);
    json.get_datetime("endTime", v.endTime);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, OrderNewBulkParams &v)
{
    json.get("orders", v.orders);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, OrderClosePositionParams &v)
{
    json.get("symbol", v.symbol);
    json.get("price", v.price);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, OrderCancelAllParams &v)
{
    json.get("symbol", v.symbol);
    json.get("filter", v.filter);
    json.get("text", v.text);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, OrderCancelAllAfterParams &v)
{
    json.get("timeout", v.timeout);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, OrderBookGetL2Params &v)
{
    json.get("symbol", v.symbol);
    json.get("depth", v.depth);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, PositionGetParams &v)
{
    json.get("filter", v.filter);
    json.get("columns", v.columns);
    json.get("count", v.count);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, PositionIsolateMarginParams &v)
{
    json.get("symbol", v.symbol);
    json.get("enabled", v.enabled);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, PositionUpdateRiskLimitParams &v)
{
    json.get("symbol", v.symbol);
    json.get("riskLimit", v.riskLimit);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, PositionTransferIsolatedMarginParams &v)
{
    json.get("symbol", v.symbol);
    json.get("amount", v.amount);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, PositionUpdateLeverageParams &v)
{
    json.get("symbol", v.symbol);
    json.get("leverage", v.leverage);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, QuoteGetParams &v)
{
    json.get("symbol", v.symbol);
    json.get("filter", v.filter);
    json.get("columns", v.columns);
    json.get("count", v.count);
    json.get("start", v.start);
    json.get("reverse", v.reverse);
    json.get_datetime("startTime", v.startTime);
    json.get_datetime("endTime", v.endTime);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, QuoteGetBucketedParams &v)
{
    json.get("binSize", v.binSize);
    json.get("partial", v.partial);
    json.get("symbol", v.symbol);
    json.get("filter", v.filter);
    json.get("columns", v.columns);
    json.get("count", v.count);
    json.get("start", v.start);
    json.get("reverse", v.reverse);
    json.get_datetime("startTime", v.startTime);
    json.get_datetime("endTime", v.endTime);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, SchemaGetParams &v)
{
    json.get("model", v.model);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, SettlementGetParams &v)
{
    json.get("symbol", v.symbol);
    json.get("filter", v.filter);
    json.get("columns", v.columns);
    json.get("count", v.count);
    json.get("start", v.start);
    json.get("reverse", v.reverse);
    json.get_datetime("startTime", v.startTime);
    json.get_datetime("endTime", v.endTime);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, TradeGetParams &v)
{
    json.get("symbol", v.symbol);
    json.get("filter", v.filter);
    json.get("columns", v.columns);
    json.get("count", v.count);
    json.get("start", v.start);
    json.get("reverse", v.reverse);
    json.get_datetime("startTime", v.startTime);
    json.get_datetime("endTime", v.endTime);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, TradeGetBucketedParams &v)
{
    json.get("binSize", v.binSize);
    json.get("partial", v.partial);
    json.get("symbol", v.symbol);
    json.get("filter", v.filter);
    json.get("columns", v.columns);
    json.get("count", v.count);
    json.get("start", v.start);
    json.get("reverse", v.reverse);
    json.get_datetime("startTime", v.startTime);
    json.get_datetime("endTime", v.endTime);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, UserGetDepositAddressParams &v)
{
    json.get("currency", v.currency);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, UserGetWalletParams &v)
{
    json.get("currency", v.currency);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, UserGetWalletHistoryParams &v)
{
    json.get("currency", v.currency);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, UserGetWalletSummaryParams &v)
{
    json.get("currency", v.currency);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, UserMinWithdrawalFeeParams &v)
{
    json.get("currency", v.currency);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, UserRequestWithdrawalParams &v)
{
    json.get("otpToken", v.otpToken);
    json.get("currency", v.currency);
    json.get("amount", v.amount);
    json.get("address", v.address);
    json.get("fee", v.fee);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, UserCancelWithdrawalParams &v)
{
    json.get("token", v.token);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, UserConfirmWithdrawalParams &v)
{
    json.get("token", v.token);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, UserRequestEnableTFAParams &v)
{
    json.get("type", v.type);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, UserConfirmEnableTFAParams &v)
{
    json.get("type", v.type);
    json.get("token", v.token);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, UserDisableTFAParams &v)
{
    json.get("type", v.type);
    json.get("token", v.token);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, UserConfirmParams &v)
{
    json.get("token", v.token);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, UserCheckReferralCodeParams &v)
{
    json.get("referralCode", v.referralCode);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, UserSavePreferencesParams &v)
{
    json.get("prefs", v.prefs);
    json.get("overwrite", v.overwrite);
    return 0;
}

template<>
int from_json(JsonUtils::Json &json, UserGetMarginParams &v)
{
    json.get("currency", v.currency);
    return 0;
}

