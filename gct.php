<?php
  require_once("kgrbe.inc");
  require_once("kgrdebug.inc");
    
  $kgrdebugobj = new kgr_debug();
  $kgrdebug = $kgrdebugobj->debug;
  $image_path = "images/";
  $dbid = "hg";
  $catbreak = 5; // Number of categories per row less 1, so 5 means 6

  $catcolumn = "col-sm-1";
  $catwidth = "120";
  $catheight = "96";
    
  if ( $kgrdebug == 1 )
  {
    echo "initload.php<br/>";
  }
    
  if ( isset($_REQUEST["aaa_notreallyanything"]) )
  {
    echo "No Se! - 1"; 
  }
  else if ( isset($_REQUEST["searchparent"]) )
  {
    if ( isset($_REQUEST["linkids"]) )
    {
      $LINKIDS = $_REQUEST["linkids"];
    }
    else
    {
      echo "Got Mary? &reg; 1";
      return;
    }
  
    if ( isset($_REQUEST["dlm"]) )
    {
      $DLM = $_REQUEST["dlm"];
    }
    else
    {
      echo "Got Mary? &reg; 2";
      return;
    }
  
    $PARENTID = $_REQUEST["searchparent"];
    
    if (file_exists("files/search".$PARENTID."srch") )
    {
      readfile("files/search".$PARENTID."srch");
      return;
    }

    $be1 = new kgr_backend("sp".$dbid);
    
    if ($be1->RWECONN == 1)
    { 
      $strLinks = explode($DLM, $LINKIDS);
      $numelems = sizeof($strLinks);

      if ($numelems == 1)
      {
        echo "Something wrong here";
        return;
      }
      
      $NEWLINKIDS = "";
      $strSrchLink = "";
      $srchdesc = "";
      $hdrimg = "<img src=\"images/IMGSRCHERE\" title=\"IMGTITLEHERE\" class=\"notimg-responsive\" height=\"100\">";
      $hdrimgsrc = "search_apparel.jpg";
      $hdrimgtitle = "Apparel Items";

      for ($i=1; $i<$numelems; $i++)
      {
        $ID = $strLinks[$i];
        $NEWLINKIDS = $NEWLINKIDS.$DLM.$ID;
        
        // echo $ID." -- ".$be1->numrows."<br/>";
        $be1->get_detail_name($ID);
        
        if ($be1->numrows > 0)
        {
          // $pid = $be1->aresultset[0][0];
          // $srchname = strtoupper($be1->aresultset[0][0]);
          // $srchhdr = strtoupper($be1->aresultset[0][1]);
          $srchname = strtoupper($be1->aresultset[0][1]);
          $srchhdr = strtoupper($be1->aresultset[0][5]);
          $srchdesc = $be1->aresultset[0][6];
          $hdrimgsrc = $be1->aresultset[0][3];
          $hdrimgtitle = $be1->aresultset[0][2];
          
          // echo $ID." -- ".$hdrimgsrc." -- ".$hdrimgtitle."<br/>";
          
          if (strlen($srchhdr) > 1)
          {
            // echo "<br/><".strlen($srchhdr)."-".$srchhdr."><br/>";
            $butname = $srchhdr."<br/>".$srchname;  
          }
          else
          {
            $butname = $srchname."<br/>&nbsp;";  
          }

          $strSrchLink = $strSrchLink."<button class=\"sizemedium\" ";
          $strSrchLink = $strSrchLink."onClick=\"LoadSearch(".$ID.",'".$NEWLINKIDS."');\" ";
          $strSrchLink = $strSrchLink.">".$butname."</button>&nbsp;";
        }
        
      }

      if ($ID != "0") // && $ID != "1")
      {
        $hdrimg = str_replace("IMGSRCHERE", $hdrimgsrc, $hdrimg);
        $hdrimg = str_replace("IMGTITLEHERE", $hdrimgtitle, $hdrimg);
      }
      else
      {
        $hdrimg = "&nbsp;";
      }
      
      // echo $hdrimg."<br/>";
      $strSrchLink = $strSrchLink."<br/><strong>".$srchdesc."</strong>";
/*      
      $strSrchLink = "";
      $strSrchLinkTemp = "";
      $continue = 1;
      $numtimes = 0;
      $CHILDID = $strLinks[0];

      while ($continue == 1 && $numtimes < 5)
      {
        // $strSrchLink = $strSrchLink." <".$numtimes."-".$CHILDID.">";
        if ($CHILDID == 0)
        {
          $continue = 0;
        }
        else
        {
          $be1->get_search_link($CHILDID);
          
          if ($be1->numrows > 0)
          {
            $pid = $be1->aresultset[0][0];
            $srchname = $be1->aresultset[0][1];
            $strSrchLinkTemp = "<button class=\"sizesmallmedium\" ";
            
            if ($numtimes > 0)
            {
              $strSrchLinkTemp = $strSrchLinkTemp."onClick=\"LoadSearch(".$CHILDID.");\" ";
            }

            $strSrchLinkTemp = $strSrchLinkTemp.">".$srchname."</button>&nbsp;";
            $CHILDID = $pid;

          }  
          else
          {
            $continue = 0;
          }  
          
          $strSrchLink = $strSrchLinkTemp.$strSrchLink;

        }
        
        $numtimes++; // Safety Loop Killer
        
      }
        
      $strSrchLink = "<button class=\"sizesmallmedium\" onClick=\"LoadSearch(".$CHILDID.");\" >HOME</button>&nbsp;".$strSrchLink;
*/
      
      $be1->get_search_info($PARENTID);
        
      if ($be1->numrows > 0)
      {
        // echo "SELECT TYPE OF PRODUCT<br/>";
        $numrows = $be1->numrows;
        $startRow = "<div class=\"row\">";
        $strSrchInfo = $startRow;
        $itemCnt = 0;
        $catcolumn = "col-lg-1 col-md-1 col-sm-2 col-xs-6";
        $startCol = "<div class=\"".$catcolumn." srchobj\" style=\"text-align:center;\">";
//        $strSrchInfo = $strSrchInfo.$startCol;
      
        for ($row=0; $row < $numrows; $row++) // getting data
        {
          // gm_search_info_id, gm_search_name, gm_search_description, gm_search_image_file, gm_product_info_id
          $sid = $be1->aresultset[$row][0];
          $srchname = $be1->aresultset[$row][1];
          $srchdesc = $be1->aresultset[$row][2];
          $srchimage = $be1->aresultset[$row][3];
          $prodid = $be1->aresultset[$row][4];
          $srchhdr = $be1->aresultset[$row][5];
        
          // echo $sid." == ".$srchimage."<br/>";
          
          if ($itemCnt < 12)
          {
            // echo "div class col ".$itemCnt."<br/>";
            $itemCnt++;
          }
          else
          {
            // echo "div class row ".$itemCnt."<br/>";
            $strSrchInfo = $strSrchInfo."</div>".$startRow;
            $itemCnt = 1;
          }

//          $strSrchInfo = $strSrchInfo.$startCol."<span class=\"sizemedium\">";
          $strSrchInfo = $strSrchInfo.$startCol;
          // $strSrchInfo = $strSrchInfo."<span class=\"sizemedium\" style=\"text-align:center;\">";
          // $strSrchInfo = $strSrchInfo.$srchhdr."<br/>";
          $strSrchInfo = $strSrchInfo."<img id=\"srchid".$sid."\" src=\"images/".$srchimage."\" width=\"".$catwidth."\" height=\"".$catheight."\" title=\"".$srchdesc."\" alt=\"".$srchdesc."\" ";
          
          if ($prodid == "0")
          {
            $strSrchInfo = $strSrchInfo."onClick=\"LoadSearch(".$sid.",'".$LINKIDS.$DLM.$sid."');\"";
          }
          else
          {
            $strSrchInfo = $strSrchInfo."onClick=\"GetProductInfoPre(".$prodid.",".$sid.",'".$LINKIDS.$DLM.$sid."','".$srchimage."','".$srchdesc."');\"";
          }
          
          // $strSrchInfo = $strSrchInfo."><br/>".$srchname."</span></div>";
//          $strSrchInfo = $strSrchInfo." class=\"img-responsive\">".$srchname."</span></div>";
          $strSrchInfo = $strSrchInfo." class=\"img-responsive\"><span class=\"sizemedium\">".$srchname."</span></div>";

        }

        $strSrchInfo = $strSrchInfo."</div>";
        
        echo $strSrchLink;
        echo $DLM;  
        echo $strSrchInfo;

        
        $srchfile = fopen("files/search".$PARENTID."srch","w");
        fwrite($srchfile, $strSrchLink);
        fwrite($srchfile, $DLM);
        fwrite($srchfile, $strSrchInfo);
        fwrite($srchfile, $DLM."LASTDLM");  
        fwrite($srchfile, $hdrimg);
        fclose($srchfile);
        
        
      }
      else
      {
         echo $strSrchLink;
         echo $DLM;  
         echo "Got Mary?&reg;<br/>";
         echo $be1->LastError."<br/>";
      }
        
      echo $DLM."LASTDLM";  
      echo $hdrimg;
      
    }

  }
  else if ( isset($_REQUEST["searchchild"]) )
  {
    if ( isset($_REQUEST["linkids"]) )
    {
      $LINKIDS = $_REQUEST["linkids"];
    }
    else
    {
      echo "Got Mary? &reg; 1";
      return;
    }
  
    if ( isset($_REQUEST["dlm"]) )
    {
      $DLM = $_REQUEST["dlm"];
    }
    else
    {
      echo "Got Mary? &reg; 2";
      return;
    }
  
    if ( isset($_REQUEST["imgsrc"]) )
    {
      $hdrimgsrc = $_REQUEST["imgsrc"];
      
      if ( isset($_REQUEST["imgtitle"]) )
      {
        $hdrimgtitle = $_REQUEST["imgtitle"];
      }
      else
      {
        $hdrimgsrc = "";
        $hdrimgtitle = "";
      }
      
    }
    else
    {
      $hdrimgsrc = "";
      $hdrimgtitle = "";
    }
  
    $OrigChildID = $_REQUEST["searchchild"];
    $CHILDID = $OrigChildID;
    $hdrimg = "<img src=\"images/IMGSRCHERE\" title=\"IMGTITLEHERE\" class=\"notimg-responsive\" height=\"100\">";
    
    if (strlen($hdrimgsrc) > 0)
    {
      $hdrimg = str_replace("IMGSRCHERE", $hdrimgsrc, $hdrimg);
      $hdrimg = str_replace("IMGTITLEHERE", $hdrimgtitle, $hdrimg);
    }
    else
    {
      // echo $hdrimgsrc."KURT<br/>";
      $hdrimg = "&nbsp;";
    }
    
    $be1 = new kgr_backend("sp".$dbid);
    
    if ($be1->RWECONN == 1)
    {  
      $strLinks = explode($DLM, $LINKIDS);
      $numelems = sizeof($strLinks);

      if ($numelems == 1)
      {
        echo "Something wrong here";
        return;
      }
      
      $NEWLINKIDS = "";
      $strSrchLink = "";

      for ($i=1; $i<$numelems; $i++)
      {
        $ID = $strLinks[$i];
        $NEWLINKIDS = $NEWLINKIDS.$DLM.$ID;
        
          $be1->get_detail_name($ID);
    //    }
        
        if ($be1->numrows > 0)
        {
          // $pid = $be1->aresultset[0][0];
          // $srchname = $be1->aresultset[0][0];
          $srchname = $be1->aresultset[0][1];
          $strSrchLink = $strSrchLink."<button class=\"sizemedium\" ";
          
          if (($i+1) < $numelems)
          {
            // echo "get_detail_name ".$ID." ".$i." ".$numelems."<br/>";
            $strSrchLink = $strSrchLink."onClick=\"LoadSearch(".$ID.",'".$NEWLINKIDS."');\" ";
          }
          
          $strSrchLink = $strSrchLink.">".$srchname."</button>&nbsp;";
        }  
        
      }
     
      /* OBSOLETE     
      $be1->get_search_child_info($OrigChildID);
        
      if ($be1->numrows > 0)
      {
        // echo "SELECT TYPE OF PRODUCT<br/>";
        $numrows = $be1->numrows;
        $strSrchInfo = "<div class=\"row\">";
        $itemCnt = 0;
        $row=0;

        // gm_search_info_id, gm_search_name, gm_search_description, gm_search_image_file, gm_product_info_id
        $sid = $be1->aresultset[$row][0];
        $srchname = $be1->aresultset[$row][1];
        $srchdesc = $be1->aresultset[$row][2];
        $srchimage = $be1->aresultset[$row][3];
        $prodid = $be1->aresultset[$row][4];
        $srchhdr = $be1->aresultset[$row][5];
        
        if (itemCnt < 6)
        {
          $itemCnt++;
        }
        else
        {
          $strSrchInfo = $strSrchInfo."</div><div class=\"row\">";
          $itemCnt = 1;
        }

        $strSrchInfo = $strSrchInfo."<div class=\"".$catcolumn."\">";
        $strSrchInfo = $strSrchInfo."<span class=\"sizemedium\">".$srchhdr."</span><br/>";
        $strSrchInfo = $strSrchInfo."<img id=\"srchid".$sid."\" src=\"images/".$srchimage."\" width=\"".$catwidth."\" height=\"".$catheight."\" title=\"".$srchdesc."\" alt=\"".$srchdesc."\" ";
        
        if ($prodid == "0")
        {
          $strSrchInfo = $strSrchInfo."onClick=\"LoadSearch(".$sid.");\"";
        }
        else
        {
          $strSrchInfo = $strSrchInfo."onClick=\"GetProductInfoPre(".$prodid.",".$sid.");\"";
        }
        
        $strSrchInfo = $strSrchInfo."><br/><span class=\"sizemedium\">".$srchname."</span></div>";

        $strSrchInfo = $strSrchInfo."<div class=\"".$catcolumn."\">";
        $strSrchInfo = $strSrchInfo."<span class=\"sizemedium\">".$srchdesc."</span></div>";

        $strSrchInfo = $strSrchInfo."</div>";
        */
        
        echo $strSrchLink;
        echo $DLM;  
        echo "LASTONE".$hdrimg;
        // echo ""; // $strSrchInfo;
    
    }
    else
    {
       echo $strSrchLink;
       echo $DLM;  
       echo "Got Mary?&reg;<br/>";
       echo $be1->LastError."<br/>";
    }
        
   

  }
  else if ( isset($_REQUEST["sizebutton"]) )
  {
    $ProdId = $_REQUEST["sizebutton"];
    // echo "sizebutton - ".$ProdId."<br/>";
    
    if ( isset($_REQUEST["sizeid"]) )
    {
      $SizeId = $_REQUEST["sizeid"];
    }
    else
    {
      $SizeId = "-1";
    }
  
    if ( isset($_REQUEST["qtypg"]) )
    {
      $QTYPage = $_REQUEST["qtypg"];
    }
    else
    {
      $QTYPage = "0";
    }
  
    $be1 = new kgr_backend("sp".$dbid);
  
    if ($be1->RWECONN == 1)
    {  
      $be1->get_sizebutton_list($ProdId);
        
      if ($be1->numrows > 0)
      {
        $numrows = $be1->numrows;
        $strTheButtons = "";

        if ($numrows == 1)
        {
          $startRow = "<div class=\"row donotdisplay\">";          
        }
        else
        {
          $startRow = "<div class=\"row\">";
        }
        
        $strTheButtons = $startRow;
        $itemCnt = 0;
        $catcolumn = "col-lg-12 col-md-12 col-sm-12 col-xs-12";
        $startCol = "<div class=\"".$catcolumn."\">"; // style=\"text-align:center;\">";
      
        for ($row=0; $row < $numrows; $row++) // getting data
        {
          
          if ($itemCnt < 16)
          {
            // echo "div class col ".$itemCnt."<br/>";
            $itemCnt++;
          }
          else
          {
            // echo "div class row ".$itemCnt."<br/>";
//            $strTheButtons = $strTheButtons."<div class=\"col-lg-6 col-md-3 col-sm-4 col-xs-6\">TACO SEASONING!&nbsp;</div>".$startRow;
            $strTheButtons = $strTheButtons."</div>".$startRow;
            $itemCnt = 1;
          }

          // results: product_size_name, product_size_description, gm_product_size_link_id, product_size_sort_order 
          // $strTheButtons = $strTheButtons.$startCol;
//          $strTheButtons = $strTheButtons."<button id=\"button0".$be1->aresultset[$row][2]."\" class=\"sizemedium\" ";
//          $strTheButtons = $strTheButtons."onclick=\"processSizeButton(".$ProdId.", '0".$be1->aresultset[$row][2]."');\" >";

          $strTheButtons = $strTheButtons."<button id=\"button".$be1->aresultset[$row][2]."\" class=\"sizemedium\" ";
          $strTheButtons = $strTheButtons."onclick=\"processSizeButton(".$ProdId.", '".$be1->aresultset[$row][2]."', '".$QTYPage."');\" >";
          $strTheButtons = $strTheButtons.$be1->aresultset[$row][1]."</button>&nbsp;"; // </div>";
        }
          
//        echo "</div>".$strTheButtons; // End of Row
        echo $strTheButtons."</div>"; // End of Row
            
      }
      /*
      else if ($be1->numrows == 1)
      {
        echo "&nbsp;";
      }
      */
      else
      {
        echo "No Products Found at This Time (101)";
      }
    }
    else
    {
      echo "No Products Found at This Time (102)";
      echo $be1->LastError."<br/>";
    }
  }
  else if ( isset($_REQUEST["messagedate"]) )
  {
    // Check if Open today
    // Don't know where the server is or how its clock is set, so default to UTC
    date_default_timezone_set( "UTC" );
    // Time based on CA-PST
    $daylight_savings_offset_in_seconds = timezone_offset_get( timezone_open( "PST" ), new DateTime() );
    // $today = date("Y-m-d H-i", time()+$daylight_savings_offset_in_seconds);
    $today = date("Y-m-d", time() + $daylight_savings_offset_in_seconds);
    $xmas = substr($today, 5, 5);
    
    if ( $xmas == "12-25" )
    {
      $dw = "0";
    }
    else
    {
      $dw = date( "w", strtotime($today));
    }
    // echo "<br/>".$today."<br/>".$xmas."<br/>".$dw."<br/>";
    // $dw="1";
    
    if ( $dw == "0" )
    {
        echo "<br/><span class=\"sizemediumlarge\" align=\"center\" style=\"font-weight: bold; color: red;\">";
        echo "We are closed on ";
        
        if ($xmas == "12-25")
        {
          echo "Christmas";
        }
        else
        {
          echo "Sunday";
        }
        
        echo "<br/>You can browse, but no one is in the Shop to take your order.<br/>Check back with us tomorrow.";
        echo "</span>";
    }
    else
    { 
      $MSGDTE = $_REQUEST["messagedate"];
      $NEWMSG = "";
    
      $be1 = new kgr_backend("sp".$dbid);
      
      if ($be1->RWECONN == 1)
      { 

        $be1->get_message($MSGDTE);
          
        if ($be1->numrows > 0)
        {
          $NEWMSG = $be1->aresultset[0][1];
        }
      
      }
      // $NEWMSG = "Happy Feast of St. Ambrose!";
      echo $NEWMSG;
    }
     
  }
  else if ( isset($_REQUEST["copyright"]) )
  { 
    readfile("../kgrdata/files/copyright.txt"); 
  }
  else if ( isset($_REQUEST["footer1"]) )
  { 
    readfile("files/footer1.txt"); 
  }
  else if ( isset($_REQUEST["productinfoOBS"]) )
  {
    $ProdId = $_REQUEST["productinfo"];

    if ( isset($_REQUEST["dlm"]) )
    {
      $DLM = $_REQUEST["dlm"];
    }
    else
    {
      echo "Got Mary? &reg;";
      return;
    }
  
    $be1 = new kgr_backend("sp".$dbid);

    if ($be1->RWECONN == 1)
    {  
      $be1->get_product_info($ProdId);
        
      if ($be1->numrows > 0)
      {
        $imgs = explode("|", $be1->aresultset[0][0]);
        // echo "<br/>".$be1->aresultset[0][0]."<br/>";
        
        if (sizeof($imgs) > 1)
        {  
          for ($i = 0; $i < sizeof($imgs); $i++)
          {
            $imgs2 = explode(":", $imgs[$i]);
            
            if (sizeof($imgs2) == 2)
            {
              echo "<span style=\"margin-right: 10;\" ><img src=\"images/".$imgs2[0]."\" height=\"150\" alt=\"".$imgs2[1]."\" /></span>&nbsp;";
            }
          }
        }
        else
        {
          $imgs2 = explode(":", $be1->aresultset[0][0]);
            
          if (sizeof($imgs2) == 2)
          {
            echo "<span style=\"margin-right: 10;\" ><img src=\"images/".$imgs2[0]."\" height=\"150\" alt=\"".$imgs2[1]."\" /></span>&nbsp;";
          }
        }
        
        echo $DLM;  
        echo "<span class=\"sizemediumlarge\">";
        echo str_ireplace("<br/>","&nbsp;",$be1->aresultset[0][1]);
        echo "</span>";

        // Don't know where the server is or how its clock is set, so default to UTC
        date_default_timezone_set( "UTC" );
        // Time based on CA-PST
        $daylight_savings_offset_in_seconds = timezone_offset_get( timezone_open( "PST" ), new DateTime() );
        // $today = date("Y-m-d H-i", time()+$daylight_savings_offset_in_seconds);
        $today = date("Y-m-d", time() + $daylight_savings_offset_in_seconds);
        $xmas = substr($today, 5, 5);
        
        if ( $xmas == "12-25" )
        {
          $dw = "0";
        }
        else
        {
          $dw = date( "w", strtotime($today));
        }
         echo "<br/>".$today."<br/>".$xmas."<br/>".$dw."<br/>";
         $dw="1";
        
        if ( $dw == "0" )
        {
            echo "<br/><span class=\"sizemediumlarge\" align=\"center\" style=\"font-weight: bold; color: red;\">";
            echo "We are closed on ";
            
            if ($xmas == "12-25")
            {
              echo "Christmas";
            }
            else
            {
              echo "Sunday";
            }
            
            echo "<br/>You can browse, but no one is in the Shop to take your order.<br/>Check back with us tomorrow.";
            echo "</span>";
        }
       
            
      }
      else
      {
         echo "Got Mary? &reg;";
      }
    }
    else
    { 
      echo "Didn't work!<br/>";
      echo $be1->LastError."<br/>";
    }
  }
  else if ( isset($_REQUEST["product_OBS"]) )
  {
    $CatId = $_REQUEST["product"];
    
    if ( isset($_REQUEST["itemid"]) )
    {
      $ItemId = $_REQUEST["itemid"];
    }
    else
    {
      $ItemId = "-1";
    }
  
    $be1 = new kgr_backend("sp".$dbid);
  
    if ($be1->RWECONN == 1)
    {  
      $be1->get_product_list($CatId);
        
      if ($be1->numrows > 0)
      {
        $numrows = $be1->numrows;

        if ($numrows == 1)
        {
          echo "<span class=\"donotdisplay\">";
        }
          
        echo "SELECT PRODUCT STYLE<br/>";
        echo "<select id=\"optproduct\" onChange=\"ProductChange(this.id,1);\">";
          
        for ($row=0; $row < $numrows; $row++) // getting data
        {
          // product_info_id, product_name, product_sort_order
          $prodid = $be1->aresultset[$row][0];

          if ($ItemId == "-1")
          {
            $ItemId = $prodid;
          }

          if ($ItemId == $prodid)
          { 
            $selectItem = " selected=\"selected\"";
          }

          $prodname = $be1->aresultset[$row][1];
          $sortorder = $be1->aresultset[$row][2]; // Not Used
          
          echo "<option value=\"".$prodid."\"".$selectItem.">".$prodname."</option>";
          $selectItem = "";

        }
          
        echo "</select><br/>";
            
        if ($numrows == 1)
        {
          echo "</span>";
        }
          
      }
      else
      {
         echo "Didn't work!<br/>";
         echo $be1->LastError."<br/>";
      }
        
    }
  }
  else if ( isset($_REQUEST["buttonOBS"]) )
  {
    $btnid=$_REQUEST["button"];
    if ( $btnid >= 0 && $btnid <= 9)
    {
      readfile("files/button".$btnid.".txt"); 
    }
    else
    {
      echo "UNDER CONTRUCTION";
    }
  }
  else if ( isset($_REQUEST["categoryOBS"]) )
  {
    if ( isset($_REQUEST["dlm"]) )
    {
      $DLM = $_REQUEST["dlm"];
    }
    else
    {
      echo "Got Mary? &reg;";
      return;
    }
  
    $be1 = new kgr_backend("sp".$dbid);
    
    if ($be1->RWECONN == 1)
    {  
      $be1->get_category_list();
        
      if ($be1->numrows > 0)
      {
        // echo "SELECT TYPE OF PRODUCT<br/>";
        $numrows = $be1->numrows;
        $catdescfull = "";
        $strTheButtons = "<div class=\"row\">";
        $itemCnt = 0;
      
        for ($row=0; $row < $numrows; $row++) // getting data
        {
          // category_info_id, category_name, category_sort_order
          $catid = $be1->aresultset[$row][0];
          $catname = $be1->aresultset[$row][1];
          $sortorder = $be1->aresultset[$row][2]; // Not Used
          $catdesc = $be1->aresultset[$row][3];
          $buttonname = strtolower($be1->aresultset[$row][4]); 

          if (itemCnt < 6)
          {
            $itemCnt++;
          }
          else
          {
            $strTheButtons = $strTheButtons."</div><div class=\"row\">";
            $itemCnt = 1;
          }

          $strTheButtons = $strTheButtons."<div class=\"col-md-2 col-sm-4 col-xs-6 divrow\"><img id=\"catimageid".$catid."\" src=\"images/category_".$buttonname.".jpg\" height=\"150\" alt=\"".$catname."\" ></div>";

          // echo "<img id=\"imagecategory".$catid."\" onClick=\"CategorySelect(".$catid.");\" src=\"images\category_".$buttonname.".jpg\" alt=\"".$catname."\" height=\"50\" >&nbsp;&nbsp;";
          // results: product_size_name, product_size_description, gm_product_size_link_id, product_size_sort_order 
          // $strTheButtons = $strTheButtons."<button id=\"imagecategory".$catid."\" class=\"sizesmallmedium\" ";
          
          /*
          $strTheButtons = $strTheButtons."<button id=\"button".$catid."\" class=\"sizesmallmedium\" ";
          $strTheButtons = $strTheButtons." style=\"background-image:url('".$image_path."category_".$buttonname.".jpg'); height: 50px; width: 31px;\" ";
          $strTheButtons = $strTheButtons." title=\"".strtoupper($catname)."\" ";
//          $strTheButtons = $strTheButtons."onClick=\"CategorySelect(".$catid.",'".$image_path."category_".$buttonname.".jpg',1);\" >";
          $strTheButtons = $strTheButtons."onClick=\"CategorySelect(".$catid.",1);\" >";
          $strTheButtons = $strTheButtons."^</button>&nbsp;";

          if ($row == $catbreak)
          {
            $strTheButtons = $strTheButtons."<br/>";
          }
          
          $catdescfull = $catdescfull."<p style=\"background-color: #CCC;\" id=\"catdesc".$catid."\" class=\"donotdisplay\">".$catdesc."</p>";
*/

        }
          
        $strTheButtons = $strTheButtons."</div>";
        echo $strTheButtons;
        // $catdescfull = "";  // Don't use for now
        // echo "<br/><br/>";
        // echo $DLM;  
        // echo $catdescfull;
    
      }
/*      else if ($be1->numrows < 0)
      {
        echo "SELECT TYPE OF PRODUCT<br/>";
        $numrows = $be1->numrows;
        $catdescfull = "";
        
        for ($row=0; $row < $numrows; $row++) // getting data
        {
          // category_info_id, category_name, category_sort_order
          $catid = $be1->aresultset[$row][0];
          $catname = $be1->aresultset[$row][1];
          $sortorder = $be1->aresultset[$row][2]; // Not Used
          $catdesc = $be1->aresultset[$row][3];
          $buttonname = strtolower($be1->aresultset[$row][4]); 
          
          echo "<img id=\"imagecategory".$catid."\" onClick=\"CategorySelect(".$catid.");\" src=\"images\category_".$buttonname.".jpg\" alt=\"".$catname."\" height=\"50\" >&nbsp;&nbsp;";
          
          $catdescfull = $catdescfull."<p style=\"background-color: #CCC;\" id=\"catdesc".$catid."\" class=\"donotdisplay\">".$catdesc."</p>";

        }
          
        // $catdescfull = "";  // Don't use for now
        // echo "<br/><br/>";
        echo $DLM;  
        echo $catdescfull;
    
      } */
      else
      {
         echo "Got Mary?&reg;<br/>";
         echo $be1->LastError."<br/>";
      }
        
    }

  }
  else
  {
    echo "No Se! - 2";
  }


     
?>
