//+------------------------------------------------------------------+
//|                                             ProForexTraderEA.mq5 |
//|                                  Copyright 2024, MetaQuotes Ltd. |
//|                                             https://www.mql5.com |
//+------------------------------------------------------------------+
#property copyright "Copyright 2024, MetaQuotes Ltd."
#property link      "https://www.mql5.com"
#property version   "1.00"

//+------------------------------------------------------------------+
//| Include files                                                    |
//+------------------------------------------------------------------+
#include <Trade/Trade.mqh>

//+------------------------------------------------------------------+
//| Struct                                                           |
//+------------------------------------------------------------------+
struct Candle
  {
   long              date;        // Candle closing time
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
int make_decision(
   Candle& candles_tf_1[],
   int candles_tf_1_size,
   Candle& candles_tf_2[],
   int candles_tf_2_size,
   Candle& candles_tf_3[],
   int candles_tf_3_size,
   int position_type,
   double position_pnl,
   double position_size,
   int position_duration,
   double base_currency_conversion_rate,
   double account_balance
);
#import

//+------------------------------------------------------------------+
//| Inputs for expert                                                |
//+------------------------------------------------------------------+
input double risk_balance = 0.05; // The risk for each trade
input double stop_loss_percent = 0.001; // The stop loss in percentage
input double take_profit_percent = 0.002; // The take profit in percentage
input bool use_auto_lots = true; // Calculate position size automatically
input ENUM_TIMEFRAMES timeframe_1 = PERIOD_M5;
input int nb_fetch_candles_timeframe_1 = 25;
input ENUM_TIMEFRAMES timeframe_2 = PERIOD_M30;
input int nb_fetch_candles_timeframe_2 = 25;
input ENUM_TIMEFRAMES timeframe_3 = PERIOD_H4;
input int nb_fetch_candles_timeframe_3 = 25;
input long magic_number = 55555; // Magic number

//+------------------------------------------------------------------+
//| Constants                                                        |
//+------------------------------------------------------------------+
#define TIMER_INTERVAL 1 // in seconds

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

   gTrade.SetExpertMagicNumber(magic_number);

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
// Process only when a new bar appears
   if(!IsNewBar())
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

// Call the DLL function
   int decision = make_decision(candles_tf_1, nb_fetch_candles_timeframe_1, candles_tf_2, nb_fetch_candles_timeframe_2, candles_tf_3, nb_fetch_candles_timeframe_3, position_type, position_pnl, position_size, position_duration, base_currency_conversion_rate, account_balance); 
   Print(decision);

// Process the decision received from the DLL
// Handle the decision accordingly

   double bidPrice = SymbolInfoDouble(_Symbol, SYMBOL_BID);
   double askPrice = SymbolInfoDouble(_Symbol, SYMBOL_ASK);
   double spread = askPrice - bidPrice;

   if(decision  == 0)
     {
      return; // WAIT
     }

   if(decision == 1) // BUY
     {
      double stopLoss = bidPrice - (stop_loss_percent * bidPrice);
      double takeProfit = bidPrice + (take_profit_percent * bidPrice) + spread;
      double stopLossPips = DifferenceInPips(askPrice, stopLoss, _Symbol);
      double volume = use_auto_lots ? CalculateVolumeForTrade(_Symbol, risk_balance, stopLossPips) : CalculateLinearVolumeForTrade(_Symbol);
      gTrade.Buy(volume, _Symbol, 0, stopLoss, takeProfit);
      return;
     }

   if(decision == 2) // SELL
     {
      double stopLoss = askPrice + (stop_loss_percent * askPrice);
      double takeProfit = askPrice - (take_profit_percent * askPrice) - spread;
      double stopLossPips = DifferenceInPips(bidPrice, stopLoss, _Symbol);
      double volume = use_auto_lots ? CalculateVolumeForTrade(_Symbol, risk_balance, stopLossPips) : CalculateLinearVolumeForTrade(_Symbol);
      gTrade.Sell(volume, _Symbol, 0, stopLoss, takeProfit);
      return;
     }

   if(decision  == 3)    // CLOSE POSITION
     {
      ClosePositions(_Symbol);
      CloseOpenOrders(_Symbol);
      return;
     }
  }

//+------------------------------------------------------------------+
//| TradeTransaction function                                        |
//+------------------------------------------------------------------+
void OnTradeTransaction(const MqlTradeTransaction& trans,
                        const MqlTradeRequest& request,
                        const MqlTradeResult& result)
  {

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
         if(OrderGetString(ORDER_SYMBOL)== pSymbol)
            gTrade.OrderDelete(ticket);
        }
     }
  }

//+------------------------------------------------------------------+
//| Returns true if a new bar has appeared for a symbol/period pair  |
//+------------------------------------------------------------------+
bool IsNewBar()
  {
   static datetime priorTime = 0;
   datetime currentTime = iTime(Symbol(), Period(), 0);
   bool result = (currentTime != priorTime);
   priorTime = currentTime;
   return(result);
  }

//+------------------------------------------------------------------+
//| Calculate the lot size                                           |
//+------------------------------------------------------------------+
double CalculateLinearVolumeForTrade(string pSymbol)
  {
   double contractSize = SymbolInfoDouble(pSymbol, SYMBOL_TRADE_CONTRACT_SIZE);
   double balance = AccountInfoDouble(ACCOUNT_BALANCE);
   double lots = VerifyVolume(pSymbol, balance / (contractSize / 10));
   return(lots);
  }

//+------------------------------------------------------------------+
//| Calculate the lot size                                           |
//| @param pSymbol - The current symbol                              |
//| @param pPercent - The percent of balance to risk (0 - 1)         |
//| @param pStopLoss - The stop loss in pips                         |
//+------------------------------------------------------------------+
double CalculateVolumeForTrade(string pSymbol, double pPercent, double pStopLoss)
  {
   double tickValue = SymbolInfoDouble(pSymbol, SYMBOL_TRADE_TICK_VALUE);
   double balance = AccountInfoDouble(ACCOUNT_BALANCE);

   double risk = balance * pPercent;
   double lots = (risk / pStopLoss / tickValue) / 100;
   lots = VerifyVolume(pSymbol, lots);

   return(lots);
  }

//+------------------------------------------------------------------+
//| Verify the validity of the volume for a symbol                   |
//+------------------------------------------------------------------+
double VerifyVolume(string pSymbol, double pVolume)
  {
   double minVolume = SymbolInfoDouble(pSymbol, SYMBOL_VOLUME_MIN);
   double maxVolume = SymbolInfoDouble(pSymbol, SYMBOL_VOLUME_MAX);
   double stepVolume = SymbolInfoDouble(pSymbol, SYMBOL_VOLUME_STEP);

   double volume;
   if(pVolume < minVolume)
      volume = minVolume;
   else
      if(pVolume > maxVolume)
         volume = maxVolume;
      else
         volume = MathRound(pVolume / stepVolume) * stepVolume;

   if(stepVolume >= 0.1)
      volume = NormalizeDouble(volume, 1);
   else
      volume = NormalizeDouble(volume, 2);

   return(volume);
  }

//+------------------------------------------------------------------+
//| Convert a price difference to pips                               |
//+------------------------------------------------------------------+
double DifferenceInPips(double pPrice1, double pPrice2, string pSymbol)
  {
   double point = SymbolInfoDouble(pSymbol, SYMBOL_POINT);
   double pip = point * 10;
   return MathAbs(pPrice1 - pPrice2) / pip;
  }

//+------------------------------------------------------------------+
//| Copy the candle data to an array                                 |
//+------------------------------------------------------------------+
void CopyCandleData(Candle& pCandles[], ENUM_TIMEFRAMES pTimeframe, int pSize)
  {
   for(int i = 0; i < pSize; ++i)
     {
      pCandles[i].date = (long) iTime(_Symbol, pTimeframe, i);
      pCandles[i].open = iOpen(_Symbol, pTimeframe, i);
      pCandles[i].high = iHigh(_Symbol, pTimeframe, i);
      pCandles[i].low = iLow(_Symbol, pTimeframe, i);
      pCandles[i].close = iClose(_Symbol, pTimeframe, i);
      pCandles[i].tick_volume = iTickVolume(_Symbol, pTimeframe, i);
      pCandles[i].volume = iVolume(_Symbol, pTimeframe, i);
      pCandles[i].spread = iSpread(_Symbol, pTimeframe, i);
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
      case PERIOD_W1:
         return 3600 * 24 * 7;
      default:
         return -1;
     }
  }

//+------------------------------------------------------------------+
