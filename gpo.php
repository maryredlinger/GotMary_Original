<?php
  require_once("kgrbe.inc");
  require_once("kgrdebug.inc");
    
  $kgrdebugobj = new kgr_debug();
  $kgrdebug = $kgrdebugobj->debug;
    
  if ( $kgrdebug == 1 )
  {
    echo "getop.php<br/>";
  }
  
  $image_path = "images/";
  $dbid = "hg";
  $color_cols = 5;
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
  
  // echo "<br/>".$today."<br/>".$xmas."<br/".$tzoffset."<br/>".$dw."<br/>";
  // $dw = "1";
  // $dw = "0";
  
  if ( $dw == "0" )
  {
    
    if ($kgrdebugobj->opensunday == "1")
    {
      $closedtoday = false;
    }
    else
    {
      $closedtoday = true;
    }
    
  }
  else
  {
    $closedtoday = false;
  }
    
  if ($kgrdebugobj->showorder == "1")
  {
    $showorder = "true";
  }
  else
  {
    $showorder = "false";
  }
  
  // $closedtoday = true;

  if ( isset($_REQUEST["pid"]) )
  {
    $ProdId = $_REQUEST["pid"];
  }
  else
  {
      echo "Got Mary? &reg;";
      return;
  }
  
  if ( isset($_REQUEST["slid"]) )
  {
    $SizLnkId = $_REQUEST["slid"];
  }
  else
  {
      echo "Got Mary? &reg;";
      return;
  }
  
  if ( isset($_REQUEST["qtypg"]) )
  {
    $QTYPage = $_REQUEST["qtypg"];
  }
  else
  {
    $QTYPage = "0";
  }
  // echo "<br>".$QTYPage." - ".$SizLnkId."<br>";
  
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
    $be1->get_product_order_info($ProdId, $SizLnkId);
        
    if ($be1->numrows > 0)
    {
      $numrows = $be1->numrows;
      $strTheButtons = "";
// 0:product_name, 1:product_size_short_name, 2:attribute1_name, 3:attribute2_name, 4:product_information,  
// 5:bpaid, 6:display_style_id, 7:num_products, 8:product_info_id, 9:invoice_product_id, 10:product_tag, 
// 11:category_info_id, 12:image_sub_dir, 13:product_size_dimension, 14:color_tag, 
// 15:discount_unit_price, 16:unit_price, 17:category_description, 18:cosz_code, 19:category_code, 20:Image Qty
      
     // for ($row=0; $row < $numrows; $row++) // getting data
  /*    {
        echo "Color Stuff";
        echo $DLM; // delim
        echo "Image Stuff";
        echo $DLM; // delim
        echo "Order Stuff";
        echo $DLM; // delim
      }
    */
    
      $strColorStuff = "";
      $strImageStuff = "";
      $strImageStuffClass = "col-xs-12 col-sm-6";
      $strImageStuffStyle = "text-align:center; font-weight:bold; margin-top:30px; margin-bottom:30px;";
      $strOrderStuff = "";
      $row=0;

      $old_product_name = "";
      $old_product_size = "";
      $product_form = "";
      $tbpos=-1;
      $tableset=0;
      $row = 0;

      // $product_name = htmlspecialchars($be1->aresultset[$row][0]);
      $product_name = $be1->aresultset[$row][0];
      $product_size_short_name = $be1->aresultset[$row][1]; // Not Used
      $attribute_name1 = $be1->aresultset[$row][2];
      $attribute_name2 = $be1->aresultset[$row][3];

      $product_information = $be1->aresultset[$row][4];
      $item_link_id = strval($be1->aresultset[$row][5])."_0";
      $display_style_id = htmlspecialchars($be1->aresultset[$row][6]);
      $StockQty = $be1->aresultset[$row][7];
      $prod_info_id = $be1->aresultset[$row][8];
      $invoice_product_id = $be1->aresultset[$row][9];
      $product_tag = htmlspecialchars($be1->aresultset[$row][10]);
      $category_info_id = $be1->aresultset[$row][11];
      $product_image_file = htmlspecialchars($be1->aresultset[$row][12]);
      $product_size_dimensions = htmlspecialchars($be1->aresultset[$row][13]);
      $color_tag = htmlspecialchars($be1->aresultset[$row][14]);
      $DiscountPrice = $be1->aresultset[$row][15];
      $TruePrice = $be1->aresultset[$row][16];
      $category_information = $be1->aresultset[$row][17];
      $cosz_code = $be1->aresultset[$row][18];
      $category_code = $be1->aresultset[$row][19];
      $image_count = $be1->aresultset[$row][20];
      
      if ($category_info_id == 102)
      {
        $invoice_product_id = strtoupper($invoice_product_id.$product_tag);
      }
          
      if ($category_code == 'GM' || $category_code == 'GK' || $category_code == 'GH' || $category_code == 'GR' || $category_code == 'GY' || $category_code == 'GJ' ) 
      { // Apparel Item
        $ApparelItem = "1";
      }
      else
      {
        $ApparelItem = "0";
      }
      
      if ( $category_code == "TS" ) // Tiny Saint
      {
        $color_cols = 5;
        $TinySaintItem = "1";
      }
      else
      {
        $TinySaintItem = "0";
      }
            
      if ($category_code == "OO")
      {
        $color_tag = $color_tag.strval($be1->aresultset[$row][5]);
        $OopsItem = "1";
      }
      else
      {
        $OopsItem = "0";
      }
      
      $DiscountRate = 1.0;
      $NormalPrice = "";
 
      // If applying a discount to all apparel items, uncomment this
      if ($today > "2020-04-14" && $today < "2020-04-22")
      {
        $KnowChange = "0";
        
        if ($TinySaintItem == "1") 
        {
          $DiscountRate = 0.700;
        }
        else if ($ApparelItem == "1")
        {
          if ($prod_info_id == "1" || $prod_info_id == "10" || $prod_info_id == "12" || $prod_info_id == "31" ||
              $prod_info_id == "45" || $prod_info_id == "48" || $prod_info_id == "59" || $prod_info_id == "80" ||
              $prod_info_id == "90" || $prod_info_id == "97" || $prod_info_id == "98")
          {
            $DiscountRate = 0.500;
          }
          else
          {
            $DiscountRate = 0.800;
          }
        }
        else 
        {
            $DiscountRate = 0.900;
        }
        
        if ($KnowChange == "0")
        {
          $TruePrice2 = number_format(round($TruePrice * $DiscountRate, 2), 2);  // Discount all prices
          // echo $TruePrice." - ".$TruePrice2."<br/>";
          $DisplayPrice = "<span style=\"color:red; font-weight:bold;\">SPECIAL:</span>";
          $NormalPrice = "WAS $".$TruePrice;
          $TruePrice = $TruePrice2;
        }
      }
      else if ($today > "2020-04-14" && $today < "2020-04-19" && ($ApparelItem == "1" || $TinySaintItem == "1") ) 
//      if ($today > "2018-03-15" && $today < "2018-03-25" && ($ApparelItem == "1" || $OopsItem == "1") ) 
//      if ($today > "2018-12-07" && $today < "2018-12-13" && ($ApparelItem == "1" || $OopsItem == "1"))
      {
        $DiscountRate = 0.750;
        $TruePrice2 = number_format(round($TruePrice * $DiscountRate, 2), 2);  // Discount all prices
        // echo $TruePrice." - ".$TruePrice2."<br/>";
        $DisplayPrice = "<span style=\"color:red; font-weight:bold;\">SPECIAL:</span>";
        $NormalPrice = "WAS $".$TruePrice;
        $TruePrice = $TruePrice2;
      }
      else if ($today == "2017-12-08" && $category_info_id == "2") // Our Lady of Grace
      {
        $DiscountRate = 0.8;
        $TruePrice2 = number_format(round($TruePrice * .80, 2));  // Discount all prices
        // echo $TruePrice." - ".$TruePrice2."<br/>";
        $DisplayPrice = "<span style=\"color:red; font-weight:bold;\">SPECIAL:</span>";
        $NormalPrice = "WAS $".$TruePrice;
        $TruePrice = $TruePrice2;
      }
      else if ($today == "2017-12-12" && $category_info_id == "1") // Our Lady of Guadalupe
      {
        $DiscountRate = 0.8;
        $TruePrice2 = number_format(round($TruePrice * .80, 2));  // Discount all prices
        // echo $TruePrice." - ".$TruePrice2."<br/>";
        $DisplayPrice = "<span style=\"color:red; font-weight:bold;\">SPECIAL:</span>";
        $NormalPrice = "WAS $".$TruePrice;
        $TruePrice = $TruePrice2;
      }
      else if ($today == "2017-04-22" && $category_info_id == "3") // Divine Mercy
      {
        $DiscountRate = 0.8;
        $TruePrice2 = number_format(round($TruePrice * .80, 2));  // Discount all prices
        // echo $TruePrice." - ".$TruePrice2."<br/>";
        $DisplayPrice = "<span style=\"color:red; font-weight:bold;\">SPECIAL:</span>";
        $NormalPrice = "WAS $".$TruePrice;
        $TruePrice = $TruePrice2;
      }
      else if ($DiscountPrice < $TruePrice)
      {
        $DisplayPrice = "  Price:";
        $NormalPrice = "WAS $".$TruePrice;
        $TruePrice = $DiscountPrice;
      }
      else
      {
        $DisplayPrice = "  Price:";
      }
      
      if ($category_info_id == 101)  // Heavenly Lips
      {
        $strImageNamePre = "hlp/hlips_".str_replace("_heavenly_lips", "", $product_image_file);        
        $product_image_file = "";
        $form_p2 = "'500','650'";
        $form_w = "'500'";
        $form_h = "'650'";
      }
      else if ($category_info_id == 102)  // Pencil        
      {
        $strImageNamePre = "hlp/h".$product_image_file;        
        $product_image_file = "";
        $form_p2 = "'500','700'";
        $form_w = "'500'";
        $form_h = "'700'";
      }
      else
      {
        $strImageNamePre = $product_tag."_".$color_tag;
        $form_p2 = "'700','650'";
        $form_w = "'700'";
        $form_h = "'650'";
      }  
      
      $strImageStuff = "<div class=\"row\">";
      $strImageStuff = $strImageStuff."<div class=\"col-xs-12 col-sm-4 sizesmallmedium\" ";
      $strImageStuff = $strImageStuff."style=\"".$strImageStuffStyle."\"><p>";

      if ( $ApparelItem == "1" ) // Apparel Item
      {
        $strImageStuff = $strImageStuff."<span class=\"sizemediumlargecred\"><strong>BE SURE TO CHECK SIZE</strong></span></br>";
        $strImageStuff = $strImageStuff."Approx Size: <a href=\"".$image_path."gotmary_dimension.jpg\"";
        $strImageStuff = $strImageStuff." onclick=\"NewWindow(this.href,'mywin','400','400','no','center'); return false;\" ";
        $strImageStuff = $strImageStuff." title=\"Click for Explanation\" target=\"_blank\">".$product_size_dimensions."</a>";
      }       
      else 
      {
        $strImageStuff = $strImageStuff.$product_size_dimensions;
        // $VolPromoDiscount = $VolPromoDiscount."[#NOPROMO#]";
      }
      

      $strImageButtons = "<div>";
            
      // if ($ApparelItem == "1" || ($prod_info_id == "8" && $product_tag = "saints") || $category_info_id == "10" || $prod_info_id == "54" 
      //   || $prod_info_id == "55" || $prod_info_id == "56") // Apparel Item, Saint Stickers, Book, Medal or Bracelets
      
      if ( $image_count > 1 )
      {

        $strImageButtons = "<div class=\"col-xs-1\">"; //col-lg-2 col-md-2 col-sm-2 col-xs-3\">";
      
        $strImageButtons = $strImageButtons."<img id=\"imgshirticon1\" onclick=\"setImage(this.id, 'imgshirt');\" src=\"".$image_path.$product_image_file.$strImageNamePre.".jpg\" ";
        $strImageButtons = $strImageButtons."height=\"40\" alt=\"".$product_name."\" title=\"Click to Show\" style=\"padding-bottom: 15px;\">";
        
        $strImageButtons = $strImageButtons."<img id=\"imgshirticon2\" onclick=\"setImage(this.id, 'imgshirt');\" src=\"".$image_path.$product_image_file.$strImageNamePre."_back.jpg\" ";
        $strImageButtons = $strImageButtons."height=\"40\" alt=\"".$product_name."\" title=\"Click to Show\" style=\"padding-bottom: 15px;\">";
        
        /*
        $strImageButtons = $strImageButtons."<img class=\"donotdisplay\" id=\"imgshirticon4\" onclick=\"setImage(this.id, 'imgshirt');\" src=\"".$image_path.$product_image_file.$strImageNamePre."_4.jpg\" ";
        $strImageButtons = $strImageButtons."height=\"40\" alt=\"".$product_name."\" title=\"Click to Show\" style=\"padding-bottom: 15px;\">";

        $strImageButtons = $strImageButtons."<img class=\"donotdisplay\" id=\"imgshirticon5\" onclick=\"setImage(this.id, 'imgshirt');\" src=\"".$image_path.$product_image_file.$strImageNamePre."_5.jpg\" ";
        $strImageButtons = $strImageButtons."height=\"40\" alt=\"".$product_name."\" title=\"Click to Show\" style=\"padding-bottom: 15px;\">";
        */
        $imgshirticonclass3 = "donotdisplay";
        $imgshirticonclass4 = "donotdisplay";
        $imgshirticonclass5 = "donotdisplay";
        
        if ( $image_count > 2 )
        {
          $imgshirticonclass3 = "ok2display";

          if ( $image_count > 3 )
          {
            $imgshirticonclass4 = "ok2display";

            if ( $image_count > 4 )
            {
              // Max 5
              $imgshirticonclass5 = "ok2display";
            }
          }
        }
        
        $strImageButtons = $strImageButtons."<img id=\"imgshirticon3\" onclick=\"setImage(this.id, 'imgshirt');\" src=\"".$image_path.$product_image_file.$strImageNamePre."_3.jpg\" class=\"".$imgshirticonclass3."\" ";
        $strImageButtons = $strImageButtons."height=\"40\" alt=\"".$product_name."\" title=\"Click to Show\" style=\"padding-bottom: 15px;\">";
        $strImageButtons = $strImageButtons."<img id=\"imgshirticon4\" onclick=\"setImage(this.id, 'imgshirt');\" src=\"".$image_path.$product_image_file.$strImageNamePre."_4.jpg\" class=\"".$imgshirticonclass4."\" ";
        $strImageButtons = $strImageButtons."height=\"40\" alt=\"".$product_name."\" title=\"Click to Show\" style=\"padding-bottom: 15px;\">";
        $strImageButtons = $strImageButtons."<img id=\"imgshirticon5\" onclick=\"setImage(this.id, 'imgshirt');\" src=\"".$image_path.$product_image_file.$strImageNamePre."_5.jpg\" class=\"".$imgshirticonclass5."\" ";
        $strImageButtons = $strImageButtons."height=\"40\" alt=\"".$product_name."\" title=\"Click to Show\" style=\"padding-bottom: 15px;\">";

        $strImageButtons = $strImageButtons."</div><div>&nbsp;</div><div>";
        $strImageButtons = $strImageButtons."</div><div class=\"col-lg-1 col-md-1 col-sm-1 col-xs-1\">&nbsp;</div><div class=\"col-lg-9 col-md-9 col-sm-9 col-xs-8\">";
      }
         
      $strImageStuff = $strImageStuff."</p>".$strImageButtons;
      
      if ($category_info_id != "101" ) // No large images for Heavenly Lips
      {
        $strImageStuff = $strImageStuff."<a href=\"large_color.php\" onClick=\"return PopUpImage(this, 'spofca', ".$form_w.", ".$form_h.")\">";
      }
          
      if ($category_info_id == "101" || $category_info_id == "102")
      {
        $product_image_file = "";
      }
      
      $imgheight = "150";

      $strImageStuff = $strImageStuff."<img id=\"imgshirt\" src=\"".$image_path.$product_image_file.$strImageNamePre.".jpg\" ";
      $strImageStuff = $strImageStuff."height=\"".$imgheight."\" alt=\"".$product_name."\" title=\"Click to Enlarge\" >";
      
      if ($category_info_id != "101" )
      {
        $strImageStuff = $strImageStuff."<br/><span class=\"hidden-xs\">Click to Enlarge</span></a>";
      }
      
      $strImageStuff = $strImageStuff."</div>";

      /*
      if ( $ApparelItem == "1" ) // Apparel Item KURT HERE
      {
        $strImageStuff = $strImageStuff."<p>Check Approx Size: <a href=\"".$image_path."gotmary_dimension.jpg\"";
        $strImageStuff = $strImageStuff." onclick=\"NewWindow(this.href,'mywin','400','400','no','center'); return false;\" ";
        $strImageStuff = $strImageStuff." title=\"Click for Explanation\" target=\"_blank\">".$product_size_dimensions."</a></p>";
      }
      */

      $strImageStuff = $strImageStuff."</div>";
      $strImageStuff = $strImageStuff."<div class=\"col-xs-12 col-sm-4 sizemedium\" ";
      $strImageStuff = $strImageStuff."style=\"".$strImageStuffStyle."\"><p>&nbsp;</p>";
      $strImageStuff = $strImageStuff.$product_information;      

      $strImageStuff = $strImageStuff."<br/><br/>";
      
      if ($OopsItem == "1")
      {
        $strImageStuff = $strImageStuff."<div id=\"oopsitemid\">".$category_information."</div></div>";
      }
      else
      {
        $strImageStuff = $strImageStuff.$category_information."</div>";
      }
      
      //      $strImageStuff = $strImageStuff."<div class=\"col-xs-12 col-sm-4 col-md-8 col-lg-10\">&nbsp;</div></div>";
      $strImageStuff = $strImageStuff."</div>";
      
      $strOrderStuff = "<span class=\"sizemediumlarge\">";
        
// BEGIN KART
// BEGIN KART
// BEGIN KART
// BEGIN KART
      if ( $closedtoday == false)
      {
/*        
        if ($category_code == "GM")
        {
          $thecartcategory = "GM_Apparel";
          $thecartcategory = "GOTMARY";
        }
        else if ($category_code == "GR")
        {
          $thecartcategory = "GR_Apparel";
        }
        else if ($category_code == "GH")
        {
          $thecartcategory = "GH_Apparel";
          $thecartcategory = "DEFAULT";
        }
        else if ($category_code == "GY")
        {
          $thecartcategory = "GY_Apparel";
        }
        else if ($category_code == "GK")
        {
          $thecartcategory = "GK_Apparel";
          $thecartcategory = "SUNPRO";
        }
        else
        {
          $thecartcategory = "DEFAULT";
        } 
*/        
        $thecartcategory = "GOTMARY";
        
        $strOrderStuff = $strOrderStuff."<form method=\"post\" action=\"https://spofca.foxycart.com/cart\" accept-charset=\"utf-8\">";
        $strOrderStuff = $strOrderStuff."<input id=\"krcat\" type=\"hidden\" name=\"category\" value=\"".$thecartcategory."\" />";
        $strOrderStuff = $strOrderStuff."<input id=\"productid\" type=\"hidden\" name=\"code\" value=\"".$invoice_product_id."\">";  
        $strOrderStuff = $strOrderStuff."<input id=\"qtymax\" type=\"hidden\" name=\"quantity_max\" value=\"".$StockQty."\" > ";
      }

      
      $strOrderStuff = $strOrderStuff."<fieldset><pre><br/><br/>";
      $strOrderStuff = $strOrderStuff."<input type=\"hidden\" name=\"h:websrc\" value=\"WEBSRCGOTMARY\" \>";
      $strOrderStuff = $strOrderStuff."      Name: <input id=\"krname\" type=\"text\" readonly=\"readonly\" name=\"name\" size=\"30\" value=\"".$product_name." \">  ";
        
      if (strlen($item_link_id) > 0)
      {
        $strOrderStuff = $strOrderStuff."<input id=\"linkstuff\" class=\"cartnoshow\" type=\"hidden\" name=\"upid\" value=\"".$item_link_id."\">  ";
      }
      
      $strColorStuffNoun = " Color";
      
      if ($OopsItem == "1")
      {
        $strOrderStuff = $strOrderStuff."<br/>      Size: <input id=\"krsize\" type=\"text\" readonly=\"readonly\" name=\"size\" size=\"30\" value=\"".$attribute_name1."\">  ";
        $strOrderStuff = $strOrderStuff."<br/>      Item: <input id=\"colorstuff\" readonly=\"readonly\" type=\"text\" name=\"color\" size=\"40\" value=\"".$attribute_name2."\" />  ";
        $strColorStuffNoun = "n Item";
      }
      else if ($display_style_id == "2")
      {
        $strOrderStuff = $strOrderStuff."<br/>      Size: <input id=\"krsize\" type=\"text\" readonly=\"readonly\" name=\"size\" size=\"30\" value=\"".$attribute_name1."\">  ";
        $strOrderStuff = $strOrderStuff."<br/>     Color: <input id=\"colorstuff\" readonly=\"readonly\" type=\"text\" name=\"color\" size=\"30\" value=\"".$attribute_name2."\" />  ";
      }
      else if ($display_style_id == "1")
      {
        $strOrderStuff = $strOrderStuff."<br/>     Style: <input id=\"krsize\" type=\"text\" readonly=\"readonly\" name=\"style\" size=\"30\" value=\"".$attribute_name1."\">  ";
        $strOrderStuff = $strOrderStuff."<br/>     Color: <input id=\"colorstuff\" readonly=\"readonly\" type=\"text\" name=\"color\" size=\"30\" value=\"".$attribute_name2."\" />  ";
      }
      else if ($display_style_id == "3")
      {
        $strOrderStuff = $strOrderStuff."<br/>     Saint: <input id=\"colorstuff\" readonly=\"readonly\" type=\"text\" name=\"saint\" size=\"30\" value=\"".$attribute_name2."\" />  ";
        $strColorStuffNoun = " Saint";
      }
      else if ($display_style_id == "4")
      {
        $strOrderStuff = $strOrderStuff."<br/>    Author: <input id=\"colorstuff\" readonly=\"readonly\" type=\"text\" name=\"author\" size=\"30\" value=\"".$attribute_name2."\" />  ";
      }
      else if ($display_style_id == "0")
      {
        $strOrderStuff = $strOrderStuff."<br/>     Style: <input id=\"colorstuff\" type=\"text\" readonly=\"readonly\" name=\"style\" size=\"30\" value=\"".$attribute_name2."\">  ";
        $strColorStuffNoun = " Style";
      }
      else // undefined display_style_id 
      {
        $strOrderStuff = $strOrderStuff."<br/>     Style: <input id=\"colorstuff\" type=\"text\" readonly=\"readonly\" name=\"style\" size=\"30\" value=\"".$attribute_name2."\">  ";
        $strColorStuffNoun = " Style";
      }
      
//KURTJUNK    NOT SURE WHY 2017-12-22
      $strOrderStuff = $strOrderStuff."<br/>   ".$DisplayPrice." <input id=\"orderprice\" type=\"text\" readonly=\"readonly\" name=\"price\" size=\"10\" value=\"".$TruePrice."\">  <span id=\"normalprice\" style=\"color:blue; font-weight:bold;\">".$NormalPrice."</span>";
      
      if ( $closedtoday == true)
      {
        if ($QTYPage == "1")
        {
          $strOrderStuff = $strOrderStuff."<span id=\"stockqty\">".$StockQty."</span>";
        }

        $strOrderStuff = $strOrderStuff."<br/>";
        $strOrderStuff = $strOrderStuff."</pre></fieldset></span>";
      }
      else
      {
        $strOrderStuff = $strOrderStuff."<br/>  Quantity: <input id=\"orderqty\" name=\"quantity\" value=\"1\" size=\"3\" onchange=\"CheckStockQty(this.id)\">  ";
        
        if ($QTYPage == "1")
        {
          $strOrderStuff = $strOrderStuff."<span id=\"stockqty\">".$StockQty."</span>";
        }
        else
        {
          $strOrderStuff = $strOrderStuff."<span id=\"stockqty\" class=\"donotdisplay\">".$StockQty."</span>";
        }
        
        $strOrderStuff = $strOrderStuff."<br/>&nbsp;";
        $strOrderStuff = $strOrderStuff."<br/>          <input type=\"submit\" name=\"add\" value=\"Add to Cart\">";
        // $strOrderStuff = $strOrderStuff."<br/>&nbsp;";
        $strOrderStuff = $strOrderStuff."</pre></fieldset></form>";
        $strOrderStuff = $strOrderStuff."<center><form action=\"https://spofca.foxycart.com/cart?cart=view\" method=\"post\" accept-charset=\"utf-8\">";
        $strOrderStuff = $strOrderStuff."<input type=\"submit\" name=\"view\" value=\"View Cart/Checkout\"></form></span>";

      }

// END NEW KART
// END NEW KART
// END NEW KART
// END NEW KART
        
// BEGIN COLOR CHART
// BEGIN COLOR CHART
// BEGIN COLOR CHART
// BEGIN COLOR CHART
      if ($numrows == 1)
      {
        $strColorStuff = "&nbsp;";
      }
      else
      {
        if ($category_info_id == 101 || $category_info_id == 102)  // Heavenly Lips or Heavenly Pencils
        {
          $strColorStuffNoun = " Style:";
        }

        $colorpos = $color_cols;
        $strColorStuff = "<table><tr><th align=\"left\" colspan=\"".$color_cols."\">Select a".$strColorStuffNoun.":</th></tr>";
        
        for ($row=0; $row < $numrows; $row++)
        {
          $invoice_product_id = $be1->aresultset[$row][9];
          // $product_tag = strtoupper(htmlspecialchars($be1->aresultset[$row][10]));
          $product_tag = htmlspecialchars($be1->aresultset[$row][10]);
          $color_tag = htmlspecialchars($be1->aresultset[$row][14]);          
          $color_name = htmlspecialchars($be1->aresultset[$row][3]);
          $product_image_file = htmlspecialchars($be1->aresultset[$row][12]);
          $StockQty = $be1->aresultset[$row][7];
          $item_link_id = strval($be1->aresultset[$row][5])."_0";
          $category_information = $be1->aresultset[$row][17];
          $image_count = $be1->aresultset[$row][20];
          
          $TruePrice = $be1->aresultset[$row][16];
     
          if ($be1->aresultset[$row][15] < $TruePrice)
          {
            $TruePrice = $be1->aresultset[$row][15];
            $NormalPrice = "";
          }
          else if ($DiscountRate < 1.0)
          {
            $TruePrice2 = number_format(round($TruePrice * $DiscountRate, 2), 2);  // Discount all prices
            $NormalPrice = "WAS $".$TruePrice;
            $TruePrice = $TruePrice2;
          }
            
          if ($category_code == "OO")
          {
            $color_tag_sdt = $color_tag.strval($be1->aresultset[$row][5]);
            $OopsItem = "1";
          }
          else
          {
            $color_tag_sdt = $color_tag;
            $OopsItem = "0";
          }
          
          if ($colorpos == $color_cols)
          {
            $strColorStuff = $strColorStuff."<tr>";
          }
            
          if ($category_info_id == 101)  // Heavenly Lips
          {
            $strImageNamePre = "hlp/hlips_".str_replace("_heavenly_lips", "", $product_image_file);        
            // echo "<br/> CAT101: ".$category_info_id." - ".$strImageNamePre."</br>";
            $strColorStuff = $strColorStuff."<td><button style=\"background-image:url('".$image_path.$strImageNamePre."_btn.jpg');"; 
            $strColorStuff = $strColorStuff." width: 30px; height: 40px;\" ";        
            $strColorStuff = $strColorStuff." title=\"".strtoupper(str_replace('_',' ',$color_name))."\" ";
            $strColorStuff = $strColorStuff."onclick=\"setDataTypeHL('".$image_path."','".$strImageNamePre."','sml','".$color_name."','".$item_link_id."','".$StockQty."','".$invoice_product_id."');\"</button></td>";
          }
          else if ($category_info_id == 102)  // Pencil        
          {
            $invoice_product_id = strtoupper($invoice_product_id.$product_tag);
            $strImageNamePre = "hlp/h".$product_image_file;
            // echo "<br/> CAT102: ".$category_info_id." - ".$strImageNamePre."</br>";
            $strColorStuff = $strColorStuff."<td><button style=\"background-image:url('".$image_path.$strImageNamePre."_btn.jpg');";
            $strColorStuff = $strColorStuff." width: 25px; height: 80px;\" ";        
            $strColorStuff = $strColorStuff." title=\"".strtoupper(str_replace('_',' ',$color_name))."\" ";
            $strColorStuff = $strColorStuff."onclick=\"setDataTypeHL('".$image_path."','".$strImageNamePre."','sml','".$color_name."','".$item_link_id."','".$StockQty."','".$invoice_product_id."');\"</button></td>";
          }
          else
          {
            
            if ($TinySaintItem == "1") // Tiny Saint
            {
              $color_prefix = $product_image_file."tinysaint_sm_";
            }
            else
            {
              $color_prefix = "clr/color_";
            }

            // echo "<br/>".$image_path." - ".$color_prefix." - ".$color_tag."<br/>";
            $strColorStuff = $strColorStuff."<td><button style=\"background-image:url('".$image_path.$color_prefix.$color_tag.".jpg'); ";
            $strColorStuff = $strColorStuff." width: 30px; height: 30px;\" ";        
            $strColorStuff = $strColorStuff." title=\"".str_replace('_',' ',$color_name)."\" ";
            $strColorStuff = $strColorStuff."onclick=\"setDataType";
            
            if ($OopsItem == "1")
            {
              $strColorStuff = $strColorStuff."OOPS('".$category_information."',";
            }
            else
            {
              $strColorStuff = $strColorStuff."(";
            }
            
            $strColorStuff = $strColorStuff."'".$image_path.$product_image_file."','".$product_tag."','".$color_tag_sdt."','".$color_name."','".$item_link_id."','".$StockQty."','".$TruePrice."','".$image_count."','".$NormalPrice."');\"</button></td>";
            // $strColorStuff = $strColorStuff."onclick=\"setDataType('".$image_path."','".$product_tag."','".$color_tag."','".$color_name."','".$item_link_id."','".$StockQty."');\"</button></td>";

          }
          
          $colorpos = $colorpos - 1;
            
          if ($colorpos == 0)
          {
            $strColorStuff = $strColorStuff."</tr>";
            $colorpos = $color_cols;
          }
     
        }
        
        if ($colorpos != $color_cols)
        {
          $strColorStuff = $strColorStuff."<td align=\"left\" colspan=\"".$color_cols."\">&nbsp;</td></tr>";
        }
                
        $strColorStuff = $strColorStuff."</table>"; // End of colors
          
      }
      
// END COLOR CHART
// END COLOR CHART
// END COLOR CHART
// END COLOR CHART

      echo $strColorStuff;
      echo $DLM; // delim
      echo $strImageStuff;
      echo $DLM; // delim
      echo $strOrderStuff;
      echo $DLM; // delim
      echo $showorder;
            
    }
    else
    {
      echo "No Products Found at This Time 1. - ";
      echo $be1->LastError;
    }
  }
  else
  {
    echo "No Products Found at This Time 2. - ";
    echo $be1->LastError;
  }
       
?>
