/**********************************************************************************************
 $$$$$			 $$$$			$$$$$			$$$$$$$$			 $$$$$		  $$$$$$		
 $$$$$$ 		$$$$$$		   $$$$$$		$$$$$$$$$$$$$$$ 		$$$$$$		  $$$$$$		
  $$$$$$	   $$$$$$$		  $$$$$$	  $$$$$$$$$$$$$$$$$ 	   $$$$$$$$ 	 $$$$$$$$		
   $$$$$	  $$$$$$$$$ 	  $$$$$ 	 $$$$$$ 				   $$$$$$$$ 	 $$$$$$$$		
	$$$$$	 $$$$$$$$$$ 	 $$$$$		 $$$$$$$$$$$$$			  $$$$$$$$$$	$$$$$$$$$		
	$$$$$$	 $$$$$ $$$$ 	$$$$$		 $$$$$$$$$$$$$$$$$		  $$$$$$$$$$   $$$$$ $$$$$		
	 $$$$$	$$$$$  $$$$$   $$$$$		   $$$$$$$$$$$$$$$$$	 $$$$$$$$$$$$ $$$$$$ $$$$$		
	  $$$$ $$$$$$  $$$$$  $$$$$ 					 $$$$$$$$	 $$$$$	$$$$$ $$$$$  $$$$$		
	  $$$$$$$$$$	$$$$$$$$$$		   $$$				$$$$$	$$$$$	$$$$$$$$$$	  $$$$$ 	 
	   $$$$$$$		$$$$$$$$$		   $$$$$$		$$$$$$$$   $$$$$$	 $$$$$$$$	  $$$$$$   
	   $$$$$$$		 $$$$$$$		   $$$$$$$$$$$$$$$$$$$$    $$$$$	  $$$$$$$	   $$$$$$   
		$$$$$		 $$$$$$ 			 $$$$$$$$$$$$$$$$	   $$$$$	  $$$$$$	   $$$$$$	
                                                                                          
 * Copyright (C) 2022 - 2023, HaoQing, <970407688@qq.com>.
 * 		<https://github.com/wuxiaolie/WirelessSersorMS>   
 * 		<https://gitee.com/yang-haoqing/wireless-sersor-ms>
 *
 * This software is licensed as described in the file COPYING, which you should have received 
 * as part of this distribution.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell copies of 
 * the Software, and permit persons to whom the Software is furnished to do so, under 
 * the terms of the COPYING file.
 *
 *********************************************************************************************/

#ifndef __LIST_QUEUE_H____
#define __LIST_QUEUE_H____
#include "data_global.h"

typedef struct msg_pack
{
	char msg_type;
	char text[27];
}link_datatype;

typedef struct _node_
{
	link_datatype data;
	struct _node_ *next;
}linknode, *linklist;

extern linklist CreateEmptyLinklist ();
extern int EmptyLinklist (linklist h);
extern linklist GetLinknode (linklist h);
extern int InsertLinknode (link_datatype x);

#endif
