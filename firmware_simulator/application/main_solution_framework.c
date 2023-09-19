#if 0
/*---------------------------------------------------------------------------------------------------------------------/
Function CreateServer

Description:
Creates a new, unitialized server object.

Requires:
  - psServerList_ points to the server list where the server is added.
  - Needs enough heap space to create a ServerType object

Promises:
  - Returns TRUE if the server is created; the new server object is added to the end of psServerList_ 
  - Returns FALSE if the server cannot be created
*/
bool CreateServer(ServerType** psServerList_)
{
  ServerType* psNewServer = 0;
  ServerType** pServerParser;
  
  /* Check that we have are not at the maximum server limit */
  if(Main_u8Servers >= U8_U8_MAX_SERVERS)
  {
    return(FALSE);
  }

  /* Try to create the server object */
  
  /* Check that we are not at the maximum server limit */

  /* Server created successfully, so initialize and add to the list */
  
  /* If empty list, new node is start of list */
  
  /* If not an empty list, parse through until the last node is found */
  
  /* Success! */
  return(TRUE);
  
} /* end CreateServer() */



  while(1)
  {
    /* Watch for a drink order */
    if(bOrderDrink)
    {
      /* Parse through the list to determine if a server has space */
      psServerListParser = psServerList;
      while( (psServerListParser != NULL) && bOrderDrink)
      {
        /* Use a "for" loop to "look" at the current server's tray to see if there are any EMPTY slots */
        for(/* Loop through the tray slots */)
        {
          if(/* Found empty slot? */)
          {
            /* Found a space: choose the type of drink based on the current system count */
            
            /* Add the drink to the tray */
            
            /* Update current message to indicate a drink was ordered and flag a new message */
      
            /* Clear the drink order */
            bOrderDrink = FALSE;
            break;
          }
        } /* end for */
        
        /* Move to next server in the list in case we're still looking for a place to put the drink */
        
      } /* end while */
      
      /* If bOrderDrink is still TRUE, then the drink order is still waiting so see if another server is available */
      if(bOrderDrink)
      {
        /* See if a new server can be created */
        if(CreateServer(&psServerList))
        {
          /* Server was created so initialize and add to list; the drink will be ordered next loop iteration */
     
        }
        else
        {
          /* A new server was not created, so flag the message and clear the order */

        }
      }
    } /* end if (bOrderDrink) */

        
    /* Check if it is time to remove a drink */
    if(u32LoopCounter % U32_DRINK_SERVE_TIME == 0)
    {
      /* Select one of the servers somewhat randomly based on the number of the last drink ordered */
      psServerListParser = psServerList;
      if(psServerListParser != NULL)
      {
        for(u8 i = 0; i < (u8)eDrink; i++)
        {
          /* Parse through to the server making sure to stay in the list */
          if(psServerListParser->psNextServer != NULL)
          {
            psServerListParser = psServerListParser->psNextServer;
          }
        }
        
        /* Look through the tray to find a drink to remove and check if the tray is empty */
        for(/* Search the whole tray*/)
        {
          if(/* Look for a non-EMPTY slot */)
          {
            /* If this is the first drink found on the tray, it's the one to be removed */
            if( /* How do you track if a drink was already removed? */ )
            {
              /* If a drink hasn't been removed yet, remove the drink and queue message */
              
            }
          }
          else
          {
            /* The slot is already empty - do you need to do anything? */
          }
        }
        
        /* If the server's tray is now empty, remove the server - what do we knowabout psServerListParser here? */
        if(/* How do you know the tray is empty? */)
        {
          /* Put a pointer on the doomed node as it will be removed and put 
          the parser back to the start of the list */
          
          /* Handle if doomed node is first */
          if(/* Is the first node the doomed node? */)
          {
            /* Update the first node pointer */
          }
          else
          {
            /* Find the node just before the doomed node */
            while(/* Move through the list to find the doomed node */)
            {
              /* Disconnect the doomed node */
            }
            
            /* Connect the current node to the node after the doomed list */
            
          }
          
          /* Free the memory of the doomed node, update server count, and queue a status message */
        } /* end if(u8EmptyCount == U8_MAX_DRINKS) */
        
      } /* if(psServerListParser != NULL) */
    } /* if(u32LoopCounter % U32_DRINK_SERVE_TIME == 0) */


    /* Check to see if a new message was added */
    if(bNewMessage)
    {
      /* Clear the variable: set a breakpoint here to halt whenever a new message is posted */
      bNewMessage = FALSE;
    }

    /* Pause for the next iteration - about 1ms of instructions */
    for(u32 i = 0; i < 1700; i++);
    u32LoopCounter++;

  } /* end while(1) main super loop */
  
} /* end main() */
#endif