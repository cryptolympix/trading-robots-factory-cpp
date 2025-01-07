//+------------------------------------------------------------------+
//|                                             ProForexTraderEA.mq5 |
//|                                  Copyright 2024, MetaQuotes Ltd. |
//|                                             https://www.mql5.com |
//+------------------------------------------------------------------+
#property copyright "Copyright 2024, MetaQuotes Ltd."
#property link "https://www.mql5.com"
#property version "1.00"

//+------------------------------------------------------------------+
//| Include files                                                    |
//+------------------------------------------------------------------+
#include <Trade/Trade.mqh>

//+------------------------------------------------------------------+
//| Struct                                                           |
//+------------------------------------------------------------------+
struct Candle
  {
   long              date;          // Candle closing time
   double            open;        // Opening price
   double            high;        // Highest price
   double            low;         // Lowest price
   double            close;       // Closing price
   double            tick_volume; // Tick volume
   double            volume;      // Trade volume
   double            spread;      // Spread
  };

//+------------------------------------------------------------------+
//| DLL                                                              |
//+------------------------------------------------------------------+
#import "ProForexTraderEA.dll"
void test_dll();
double make_decision(
   Candle &candles_tf_1[],
   int candles_tf_1_size,
   int tf_1_code,
   Candle &candles_tf_2[],
   int candles_tf_2_size,
   int tf_2_code,
   Candle &candles_tf_3[],
   int candles_tf_3_size,
   int tf_3_code,
   int position_type,
   double position_pnl,
   double position_size,
   int position_duration,
   double base_currency_conversion_rate,
   double account_balance);
#import

//+------------------------------------------------------------------+
//| Inputs for expert                                                |
//+------------------------------------------------------------------+
input double risk_balance = 0.05;         // The risk for each trade
input double stop_loss_percent = 0.002;   // The stop loss in percentage
input double take_profit_percent = 0.002; // The take profit in percentage
input ENUM_TIMEFRAMES timeframe_1 = PERIOD_M15; // The shortest timeframe
input int nb_fetch_candles_timeframe_1 = 25; // The candles window for the short timeframe
input ENUM_TIMEFRAMES timeframe_2 = PERIOD_H1; // The middle timeframe
input int nb_fetch_candles_timeframe_2 = 25; // The candles window for the middle timeframe
input ENUM_TIMEFRAMES timeframe_3 = PERIOD_H4; // The long timeframe
input int nb_fetch_candles_timeframe_3 = 25; // The candles window for the long timeframe

//+------------------------------------------------------------------+
//| Constants                                                        |
//+------------------------------------------------------------------+
#define TIMER_INTERVAL 60 // in seconds

//+------------------------------------------------------------------+
//| Global variables                                                 |
//+------------------------------------------------------------------+
CTrade gTrade;

//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit()
  {
// Subscribe to tick events
   EventSetTimer(TIMER_INTERVAL); // Call OnTick() every second (adjust the frequency as needed)

   gTrade.SetExpertMagicNumber(12345);

   return INIT_SUCCEEDED;
  }

//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
  {
   EventKillTimer();
  }

//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
  {
// Process only when a new candle appears
   if(!IsNewCandle())
      return;

// Fetch historical candle data for each timeframe
   Candle candles_tf_1[];
   Candle candles_tf_2[];
   Candle candles_tf_3[];

   ArrayResize(candles_tf_1, nb_fetch_candles_timeframe_1);
   ArrayResize(candles_tf_2, nb_fetch_candles_timeframe_2);
   ArrayResize(candles_tf_3, nb_fetch_candles_timeframe_3);

   CopyCandleData(candles_tf_1, timeframe_1, nb_fetch_candles_timeframe_1);
   CopyCandleData(candles_tf_2, timeframe_2, nb_fetch_candles_timeframe_2);
   CopyCandleData(candles_tf_3, timeframe_3, nb_fetch_candles_timeframe_3);

   int position_type = 0;
   double position_pnl = 0;
   double position_size = 0;
   int position_duration = 0;

   if(PositionSelect(_Symbol))
     {
      position_type = (PositionGetInteger(POSITION_TYPE) == POSITION_TYPE_BUY) ? 1 : -1;
      position_pnl = PositionGetDouble(POSITION_PROFIT);
      position_size = PositionGetDouble(POSITION_VOLUME);
      position_duration = TradeDuration();
     }

// Base currency conversion rate
   double base_currency_conversion_rate = 1.0; // Adjust this based on your currency conversion rate

// Get the account balance
   double account_balance = AccountInfoDouble(ACCOUNT_BALANCE);

// Get timeframes codes
   int tf_1_code = TimeFrameCode(timeframe_1);
   int tf_2_code = TimeFrameCode(timeframe_2);
   int tf_3_code = TimeFrameCode(timeframe_3);

// Call the DLL function
   double decision = make_decision(candles_tf_1, nb_fetch_candles_timeframe_1, tf_1_code, candles_tf_2, nb_fetch_candles_timeframe_2, tf_2_code, candles_tf_3, nb_fetch_candles_timeframe_3, tf_3_code, position_type, position_pnl, position_size, position_duration, base_currency_conversion_rate, account_balance);

   HandleDecision(decision);
  }

//+------------------------------------------------------------------+
//| TradeTransaction function                                        |
//+------------------------------------------------------------------+
void OnTradeTransaction(const MqlTradeTransaction &trans,
                        const MqlTradeRequest &request,
                        const MqlTradeResult &result)
  {
//---
  }

//+------------------------------------------------------------------+
//| Trade function                                                   |
//+------------------------------------------------------------------+
void OnTrade()
  {
//---
  }

//+==================================================================+
//+=========================== UTILS ================================+
//+==================================================================+

//+------------------------------------------------------------------+
//| Handle the decision accordingly                                  |
//+------------------------------------------------------------------+
void HandleDecision(int decision)
  {
   double bidPrice = SymbolInfoDouble(_Symbol, SYMBOL_BID);
   double askPrice = SymbolInfoDouble(_Symbol, SYMBOL_ASK);
   double closePrice = iClose(_Symbol, PERIOD_M5, 1);
   double spread = askPrice - bidPrice;

   if(decision == 0)
     {
      return; // WAIT
     }

   if(decision == 1)  // BUY
     {
      double stopLoss = bidPrice - (stop_loss_percent * bidPrice);
      double takeProfit = bidPrice + (take_profit_percent * bidPrice);
      double stopLossPips = DifferenceInPips(bidPrice, stopLoss, _Symbol);
      double volume = CalculatePositionVolume(_Symbol, risk_balance, stopLossPips);

      // Place a Buy Limit order at the close price of the last closed candle
      // gTrade.BuyLimit(volume, closePrice, _Symbol, stopLoss, takeProfit, ORDER_TIME_DAY);
      gTrade.Buy(volume, _Symbol, 0, stopLoss, takeProfit);
      return;
     }

   if(decision == 2)  // SELL
     {
      double stopLoss = askPrice + (stop_loss_percent * askPrice);
      double takeProfit = askPrice - (take_profit_percent * askPrice);
      double stopLossPips = DifferenceInPips(askPrice, stopLoss, _Symbol);
      double volume = CalculatePositionVolume(_Symbol, risk_balance, stopLossPips);

      // Place a Sell Limit order at the close price of the last closed candle
      // gTrade.SellLimit(volume, closePrice, _Symbol, stopLoss, takeProfit, ORDER_TIME_DAY);
      gTrade.Sell(volume, _Symbol, 0, stopLoss, takeProfit);
      return;
     }

   if(decision == 3)  // CLOSE POSITION
     {
      ClosePositions(_Symbol);
      CloseOpenOrders(_Symbol);
      return;
     }
  }

//+------------------------------------------------------------------+
//| Close every open positions for a symbol                          |
//+------------------------------------------------------------------+
void ClosePositions(string pSymbol, ENUM_POSITION_TYPE pType = NULL)
  {
   for(int i = PositionsTotal() - 1; i >= 0; i--)
     {
      ulong ticket = PositionGetTicket(i);
      if(PositionSelectByTicket(ticket))
        {
         if(PositionGetString(POSITION_SYMBOL) == pSymbol && ((pType != NULL && PositionGetInteger(POSITION_TYPE) == pType) || pType == NULL))
            gTrade.PositionClose(ticket);
        }
     }
  }

//+------------------------------------------------------------------+
//| Close every open orders for a symbol                             |
//+------------------------------------------------------------------+
void CloseOpenOrders(string pSymbol)
  {
   for(int i = OrdersTotal() - 1; i >= 0; i--)
     {
      ulong ticket = OrderGetTicket(i);
      if(OrderSelect(ticket))
        {
         if(OrderGetString(ORDER_SYMBOL) == pSymbol)
            gTrade.OrderDelete(ticket);
        }
     }
  }

//+------------------------------------------------------------------+
//| Check if a new candle has formed                                 |
//+------------------------------------------------------------------+
bool IsNewCandle()
  {
   static datetime lastCandleTime = 0;

// Get the current time
   datetime currentTime = TimeCurrent();

// Get the time of the most recent complete candle
   datetime currentCandleTime = iTime(_Symbol, Period(), 0);

// Check if the current candle's time is different from the last candle's time
   if(currentCandleTime != lastCandleTime)
     {
      // Update the last candle's time
      lastCandleTime = currentCandleTime;
      return true; // A new candle has formed
     }

   return false; // No new candle has formed
  }

//+------------------------------------------------------------------+
//| Calculate the position volume in lots                            |
//+------------------------------------------------------------------+
double CalculatePositionVolume(double pAccountEquity, double pRiskPercentage, double pStopLossPips, const double pBaseCurrencyConversionRate = 1.0)
  {
// Get lot step and min lot size for the current symbol
   double lotStep = SymbolInfoDouble(_Symbol, SYMBOL_VOLUME_STEP);

// Get the account balance
   double accountBalance = AccountInfoDouble(ACCOUNT_BALANCE);

// Calculate pip value
   double pipValue = SymbolInfoDouble(_Symbol, SYMBOL_TRADE_TICK_VALUE);

// Calculate position volume
   double volume = (accountBalance * pRiskPercentage) / (pStopLossPips * pipValue);

// Check the validity of the volume
   volume = VerifyVolume(_Symbol, volume);

   return volume;
  }

//+------------------------------------------------------------------+
//| Verify the validity of the volume for a symbol                   |
//+------------------------------------------------------------------+
double VerifyVolume(string pSymbol, double pVolume)
  {
// Get the minimum and maximum volume allowed by the broker
   double minVolume = SymbolInfoDouble(pSymbol, SYMBOL_VOLUME_MIN);
   double maxVolume = SymbolInfoDouble(pSymbol, SYMBOL_VOLUME_MAX);

// Get the volume step size
   double stepVolume = SymbolInfoDouble(pSymbol, SYMBOL_VOLUME_STEP);

// Ensure volume is within the broker's limits
   if(pVolume < minVolume)
      pVolume = minVolume;
   else
      if(pVolume > maxVolume)
         pVolume = maxVolume;

// Ensure volume adheres to the volume step size
   pVolume = MathRound(pVolume / stepVolume) * stepVolume;

   return pVolume;
  }

//+------------------------------------------------------------------+
//| Convert a price difference to pips                               |
//+------------------------------------------------------------------+
double DifferenceInPips(double pPrice1, double pPrice2, string pSymbol)
  {
// Calculate the point size
   double pointSize = SymbolInfoDouble(pSymbol, SYMBOL_POINT);

// Calculate the stop loss distance in pips
   int stopLossPips = MathRound(MathAbs(pPrice1 - pPrice2) / pointSize);

   return stopLossPips;
  }

//+------------------------------------------------------------------+
//| Copy the candle data to an array                                 |
//+------------------------------------------------------------------+
void CopyCandleData(Candle &pCandles[], ENUM_TIMEFRAMES pTimeframe, int pSize)
  {
   for(int i = 0; i < pSize; ++i)
     {
      pCandles[pSize - 1 - i].date = (long)iTime(_Symbol, pTimeframe, i);
      pCandles[pSize - 1 - i].open = iOpen(_Symbol, pTimeframe, 1 + i);
      pCandles[pSize - 1 - i].high = iHigh(_Symbol, pTimeframe, 1 + i);
      pCandles[pSize - 1 - i].low = iLow(_Symbol, pTimeframe, 1 + i);
      pCandles[pSize - 1 - i].close = iClose(_Symbol, pTimeframe, 1 + i);
      pCandles[pSize - 1 - i].tick_volume = iVolume(_Symbol, pTimeframe, 1 + i);
      pCandles[pSize - 1 - i].volume = iVolume(_Symbol, pTimeframe, 1 + i);
      pCandles[pSize - 1 - i].spread = iSpread(_Symbol, pTimeframe, 1 + i);
     }
  }

//+--------------------------------------------------------------------------------------+
//| Calculate the duration of the current trade in unit of candles in the main timeframe |                                                                 |
//+--------------------------------------------------------------------------------------+
int TradeDuration()
  {
// Check if there is an open position
   if(PositionSelect(_Symbol) == true)
     {
      // Get the time the position was opened
      datetime openTime = PositionGetInteger(POSITION_TIME);

      // Get the current time
      datetime currentTime = TimeCurrent();

      // Calculate the duration in seconds
      int durationSeconds = (int)(currentTime - openTime);

      // Get the duration in seconds for a candle of the main timeframe
      int durationTimeFrameSeconds = TimeFrameToSeconds(PERIOD_CURRENT);

      return (durationSeconds / durationTimeFrameSeconds);
     }

   return 0;
  }

//+------------------------------------------------------------------+
//| Returns the seconds for a timeframe                              |
//+------------------------------------------------------------------+
int TimeFrameToSeconds(ENUM_TIMEFRAMES pTimeFrame)
  {
   switch(pTimeFrame)
     {
      case PERIOD_M1:
         return 60;
      case PERIOD_M2:
         return 60 * 2;
      case PERIOD_M3:
         return 60 * 3;
      case PERIOD_M4:
         return 60 * 4;
      case PERIOD_M5:
         return 60 * 5;
      case PERIOD_M6:
         return 60 * 6;
      case PERIOD_M10:
         return 60 * 10;
      case PERIOD_M12:
         return 60 * 12;
      case PERIOD_M15:
         return 60 * 15;
      case PERIOD_M20:
         return 60 * 20;
      case PERIOD_M30:
         return 60 * 30;
      case PERIOD_H1:
         return 3600;
      case PERIOD_H2:
         return 3600 * 2;
      case PERIOD_H3:
         return 3600 * 3;
      case PERIOD_H4:
         return 3600 * 4;
      case PERIOD_H6:
         return 3600 * 6;
      case PERIOD_H8:
         return 3600 * 8;
      case PERIOD_D1:
         return 3600 * 24;
      default:
         return -1;
     }
  }

//+------------------------------------------------------------------+
//| Return a code for the a given timeframe                        |
//+------------------------------------------------------------------+
int TimeFrameCode(ENUM_TIMEFRAMES timeframe)
  {
   switch(timeframe)
     {
      case PERIOD_M1:
         return 0;
      case PERIOD_M5:
         return 1;
      case PERIOD_M15:
         return 2;
      case PERIOD_M30:
         return 3;
      case PERIOD_H1:
         return 4;
      case PERIOD_H4:
         return 5;
      case PERIOD_D1:
         return 6;
      default:
         return -1;
     }
  }

//+------------------------------------------------------------------+
