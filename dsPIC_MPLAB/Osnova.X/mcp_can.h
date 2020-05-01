
/*! \file mcp_can.h
    \brief Deklaracija funkcija za MCP2515
    
 U ovom fajlu deklarisane su sve funkcije implementirane u mcpcan.c 
 *Biblioteka je ekvivalentna Arduino biblioteci !

*/

#ifndef MCP_CAN_H
#define MCP_CAN_H

#include "mcp_can_dfs.h"          

/*! \def MAX_CHAR_IN_MESSAGE
    \brief Velicina niza
   
 Broj karaktera u nizu poruka koje se razmenjuju u CAN komunikaciji.
*/
#define MAX_CHAR_IN_MESSAGE 8       


    /// Pin inicijalizacija za SPI.
    void InitSPIPin(void); 

    /// Inicijalizacija registara za SPI.
    void InitSPI (void);    
    
    /// Citanje spi. Salje se 0x00.
    unsigned int spi_read(void);    
    
    /// Razmena podatka preko SPI.
    unsigned int spi_readwrite(unsigned int data);  
    
    /// Zatvaranje SPI porta.
    void SPIClose(void);    
    
    /// delay funkcija preko for petlje
    void delayMicroseconds1 (int kasnjenje);        
    
    /// Soft Reset MCP2515
    void mcp2515_reset(void);                                           

    /// Read MCP2515 register
    INT8U mcp2515_readRegister(const INT8U address);                    
    
    /// Read MCP2515 successive registers
    void mcp2515_readRegisterS(const INT8U address,                     
	                             INT8U values[], 
                               const INT8U n);
     /// Set MCP2515 register
    void mcp2515_setRegister(const INT8U address,                      
                             const INT8U value);

    /// Set MCP2515 successive registers
    void mcp2515_setRegisterS(const INT8U address,                      
                              const INT8U values[],
                              const INT8U n);
    
     /// Inicijalizacija CAN bafera
    void mcp2515_initCANBuffers(void);                              
    
    /// Set specific bit(s) of a register
    void mcp2515_modifyRegister(const INT8U address,                    
                                const INT8U mask,
                                const INT8U data);

    /// Read MCP2515 Status
    INT8U mcp2515_readStatus(void);                                    
    
    /// Set mode
    INT8U mcp2515_setCANCTRL_Mode(const INT8U newmode);                 
    
    /// Set baud rate
    INT8U mcp2515_configRate(const INT8U canSpeed,                      
                             const INT8U canClock);
    /// Initialize Controller                       
    INT8U mcp2515_init(const INT8U canIDMode,const INT8U canSpeed,const INT8U canClock);                       
	
    /// Write CAN Mask or Filter
    void mcp2515_write_mf( const INT8U mcp_addr,const INT8U ext,const INT32U id ); 
	
    /// Write CAN ID
    void mcp2515_write_id( const INT8U mcp_addr,                        
                           const INT8U ext,
                           const INT32U id );

    /// Read CAN ID
    void mcp2515_read_id( const INT8U mcp_addr,                         
				INT8U* ext,
                                INT32U* id );
    /// Write CAN message
    void mcp2515_write_canMsg( const INT8U buffer_sidh_addr );  
    
    /// Read CAN message
    void mcp2515_read_canMsg( const INT8U buffer_sidh_addr); 
    
    /// Find empty transmit buffer
    INT8U mcp2515_getNextFreeTXBuf(INT8U *txbuf_n);                     

/*********************************************************************************************************
 *  CAN operator function
 *********************************************************************************************************/
    
     /// Set message
    INT8U setMsg(INT32U id, INT8U rtr, INT8U ext, INT8U len, INT8U *pData);  
    
    /// Clear all message to zero
    INT8U clearMsg();  
    
    /// Read message
    INT8U readMsg(); 
    
    /// Send message
    INT8U sendMsg();                                                    


    //MCP_CAN(INT8U _CS);
    /// Initialize controller parameters
    INT8U begin(INT8U idmodeset, INT8U speedset, INT8U clockset);       
   // INT8U init_Mask(INT8U num, INT8U ext, INT32U ulData);               // Initialize Mask(s)
    
    /// Initialize Mask(s) Ne koristi se !!!
    INT8U init_Mask(INT8U num, INT32U ulData);                          
   // INT8U init_Filt(INT8U num, INT8U ext, INT32U ulData);               // Initialize Filter(s)
    
     /// Initialize Filter(s) Ne koristi se !!!
    INT8U init_Filt(INT8U num, INT32U ulData);  
    
    /// Set operational mode
    INT8U setMode(INT8U opMode);                                        
   // INT8U sendMsgBuf(INT32U id, INT8U ext, INT8U len, INT8U *buf);      // Send message to transmit buffer
    
     /// Send message to transmit buffer
    INT8U sendMsgBuf(INT32U id, INT8U len, INT8U *buf);                
    //INT8U readMsgBuf(INT32U *id, INT8U *ext, INT8U *len, INT8U *buf);   // Read message from receive buffer
    
     /// Read message from receive buffer
    INT8U readMsgBuf(INT32U *id, INT8U *len, char *buf);   
    
     /// Check for received data
    INT8U checkReceive(void);  
    
     /// Check for errors
    INT8U checkError(void);   
    
     /// Check for errors
    INT8U getError(void);
    
    /// Get error count
    INT8U errorCountRX(void); 
    
    /// Get error count
    INT8U errorCountTX(void);
    
    /// Enable one-shot transmission
    INT8U enOneShotTX(void);    
    
    /// Disable one-shot transmission
    INT8U disOneShotTX(void); 
    
    /// Abort queued transmission(s)
    INT8U abortTX(void); 
    
    /// Sets GPO
    INT8U setGPO(INT8U data);  
    
    /// Reads GPI
    INT8U getGPI(void);                                                 

#endif
    
/*********************************************************************************************************
 *  END FILE
 *********************************************************************************************************/
