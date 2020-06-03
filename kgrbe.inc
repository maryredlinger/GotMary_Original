<?php
require_once("kgrda.inc");
require_once("kgrdebug.inc");

class kgr_backend
{                                                   
    public $aresultset;
    public $numcols;
    public $numrows;
    public $rs_numcols;
    public $rs_numrows;
    public $RWECONN;
    public $LastError;

    private $da1;
    private $qresultset;
    private $DBType = "MYSQL";
    private $MSSQLType = "MSSQL";
    private $MYSQLType = "MYSQL";
    private $fok;
    private $maxtop = "25";
    private $dbg;
    
    function __construct($thedb)
    {
        $this->dbg = new kgr_debug();
        $this->numcols = -2;
        $this->numrows = -2;
        $this->qresultset = NULL;
        $this->da1 = new kgr_dataaccess($this->MYSQLType, $thedb);
        // $this->da1 = new kgr_dataaccess($this->MYSQLType, 'sp615');

        if ($this->da1->RWECONN != 1)
        {
            $this->da1 = null;
            $this->RWECONN = 0;
        }
        else
        {
            $this->RWECONN = $this->da1->RWECONN;
        }

    }

    function __destruct()
    {
        $this->da1 = null;
    }

    public function get_message($TheDate)
    {
        $this->fok = 0;
        $query = "SELECT bs_message_name, bs_message_text ";
        $query = $query."FROM bs_message_info ";
        $query = $query."WHERE bs_message_status = 1 ";
        $query = $query."  AND bs_message_from_date <= '".$TheDate."' ";
        $query = $query."  AND bs_message_thru_date >= '".$TheDate."'; ";

        // echo $query;
        
        $this->execute_query($query);

        if ($this->fok == 1)
        {
            $this->make_array($this->qresultset, false, false);
        }
        
        if ( isset($this->qresultset) )
        {
          $this->qresultset->free();
          unset($this->qresultset);
        }

        $this->qresultset = null;
        
    }

    public function get_detail_name($ID)
    {
      // echo "get_detail_name: ";
        $query = "  AND bsd.bs_search_detail_info_id = ".$ID." ";
        $this->get_search_detail_info($query);
    }

    public function get_search_info($ParentID)
    {
      // echo "get_search_info: ";
        $query = "  AND bsd.bs_search_detail_parent_id = ".$ParentID." ";
        $this->get_search_detail_info($query);

    }

    public function get_search_child_info($ChildID)
    {
      // echo "get_search_child_info: ";
        $query = "  AND bsd.bs_search_detail_info_id = ".$ChildID." ";
        $this->get_search_detail_info($query);
        
    }

    private function get_search_detail_info($Where)
    {
        $this->fok = 0;
        $query = "SELECT bsd.bs_search_detail_info_id, bsd.bs_search_detail_name, bsd.bs_search_detail_description, bsd.bs_search_detail_image_file, ";
        $query = $query."bsd.bs_product_id, bsd.bs_search_detail_header, IFNULL(bsda.bs_search_detail_added_description, '') AS list_description ";
        $query = $query."FROM bs_search_detail_info bsd ";
        $query = $query."LEFT JOIN bs_search_detail_added_info bsda ";
        $query = $query." ON bsd.bs_search_detail_info_id = bsda.bs_search_detail_info_id ";
        $query = $query."WHERE bs_search_detail_status = 1 ";
        $query = $query." ".$Where." ";
        $query = $query."ORDER BY bsd.bs_search_detail_sort_order, bsd.bs_search_detail_name; ";

        // echo $query."<br/>";
        
        $this->execute_query($query);

        if ($this->fok == 1)
        {
            $this->make_array($this->qresultset, false, false);
        }
        
        if ( isset($this->qresultset) )
        {
          $this->qresultset->free();
          unset($this->qresultset);
        }

        $this->qresultset = null;
        
    }

    public function get_sizebutton_list($gmprodid)
    {
        $this->fok = 0;

        $strWhere = " ";
        
        if ($gmprodid == 201)
        {
          // Heavenly Lips
          $strWhere = " AND chi.category_hl_info_id < 12 ";
          $query = "SELECT DISTINCT ";
          $query = $query."   chi.category_hl_name AS product_size_name, chi.category_hl_name AS product_size_description, ";
          $query = $query."  chi.category_hl_info_id, chi.category_sort_order ";
          $query = $query."FROM category_hl_info chi ";
          $query = $query."INNER JOIN hl_category_product_link hcpl "; 
          $query = $query."  ON chi.category_hl_info_id = hcpl.category_hl_info_id "; 
          $query = $query."    AND chi.category_status = 0 "; 
          $query = $query."    AND chi.category_hl_info_id <> 0 ";
          $query = $query."INNER JOIN product_hl_info phi "; 
          $query = $query."  ON hcpl.product_hl_info_id = phi.product_info_id "; 
          $query = $query."    AND phi.product_status = 0 ";
          $query = $query."WHERE 1 = 1 ".$strWhere; // No Pencils
          $query = $query."ORDER BY chi.category_sort_order ";
        }
        else if ($gmprodid == 202)
        {
          // Heavenly Pencils
          $strWhere = " AND chi.category_hl_info_id = 12 ";
          $query = "SELECT DISTINCT ";
          $query = $query."   chi.category_hl_name AS product_size_name, chi.category_hl_name AS product_size_description, ";
          $query = $query."  chi.category_hl_info_id, chi.category_sort_order ";
          $query = $query."FROM category_hl_info chi ";
          $query = $query."INNER JOIN hl_category_product_link hcpl "; 
          $query = $query."  ON chi.category_hl_info_id = hcpl.category_hl_info_id "; 
          $query = $query."    AND chi.category_status = 0 "; 
          $query = $query."    AND chi.category_hl_info_id <> 0 ";
          $query = $query."INNER JOIN product_hl_info phi "; 
          $query = $query."  ON hcpl.product_hl_info_id = phi.product_info_id "; 
          $query = $query."    AND phi.product_status = 0 ";
          $query = $query."WHERE 1 = 1 ".$strWhere; // Pencils
          $query = $query."ORDER BY chi.category_sort_order ";
        }
        else if ($gmprodid == 61 || $gmprodid == 62|| $gmprodid == 63|| $gmprodid == 64|| $gmprodid == 65) 
        {
          $query = "SELECT DISTINCT ";
          $query = $query."  bas.bs_attributes_short_name, ";
          $query = $query."  CASE WHEN bas.bs_attributes_short_name = 'S' THEN 'Small' ";
          $query = $query."       WHEN bas.bs_attributes_short_name = 'M' THEN 'Medium' ";
          $query = $query."       WHEN bas.bs_attributes_short_name = 'L' THEN 'Large' ";
          $query = $query."       WHEN bas.bs_attributes_short_name = 'XL' THEN 'X-Large' ";
          $query = $query."       WHEN bas.bs_attributes_short_name = '2XL' THEN '2X-Large' ";
          $query = $query."       WHEN bas.bs_attributes_short_name = '3XL' THEN '3X-Large' ";
          $query = $query."       WHEN bas.bs_attributes_short_name = 'LS' THEN 'Ladies Small' ";
          $query = $query."       WHEN bas.bs_attributes_short_name = 'LM' THEN 'Ladies Medium' ";
          $query = $query."       WHEN bas.bs_attributes_short_name = 'LL' THEN 'Ladies Large' ";
          $query = $query."       WHEN bas.bs_attributes_short_name = 'LXL' THEN 'Ladies X-Large' ";
          $query = $query."       WHEN bas.bs_attributes_short_name = 'L2XL' THEN 'Ladies 2X-Large' ";
          $query = $query."       WHEN bas.bs_attributes_short_name = 'YL' THEN 'Youth Large' ";
          $query = $query."       ELSE bas.bs_attributes_short_name END button_size_name, ";
          $query = $query."  bs_attributes_short_name AS bs_attributes_id, bas.bs_attributes_sort_order ";
          $query = $query."FROM bs_product_attribute_info bpai ";
          $query = $query."INNER JOIN bs_attributes_info bas ";
          $query = $query."    ON bpai.bs_attributes_id = bas.bs_attributes_id ";
          $query = $query."      AND bpai.bs_product_attribute_status = 1 ";
          $query = $query."      AND bpai.bs_product_attribute_quantity > 0 ";
          $query = $query."      AND bas.bs_attributes_status = 1 ";
          $query = $query."LEFT JOIN (SELECT gm_product_size_link_id AS bs_product_attribute_id, SUM(product_quantity) AS product_quantity ";
          $query = $query."           FROM gm_post_purchase ";
          $query = $query."           WHERE product_color_id = 0 AND transaction_applied = '0' ";
          $query = $query."           GROUP BY gm_product_size_link_id) gpp ";
          $query = $query."    ON bpai.bs_product_attribute_id = gpp.bs_product_attribute_id ";
          $query = $query."WHERE 1 = 1 ";
          $query = $query."  AND bpai.bs_product_id = ".$gmprodid." ";
          $query = $query."  AND (bpai.bs_product_attribute_quantity - IFNULL(gpp.product_quantity,0)) > 0 ";
          $query = $query."ORDER BY bas.bs_attributes_sort_order, bas.bs_attributes_id ";
        }
        else
        {
          $query = "SELECT DISTINCT ";
          $query = $query."  bas.bs_attributes_short_name, bas.bs_attributes_name, ";
          $query = $query."  bas.bs_attributes_id, bas.bs_attributes_sort_order ";
          $query = $query."FROM bs_product_attribute_info bpai ";
          $query = $query."INNER JOIN bs_attributes_info bas ";
          $query = $query."    ON bpai.bs_attributes_id = bas.bs_attributes_id ";
          $query = $query."      AND bpai.bs_product_attribute_status = 1 ";
          $query = $query."      AND bpai.bs_product_attribute_quantity > 0 ";
          $query = $query."      AND bas.bs_attributes_status = 1 ";
          // $query = $query."LEFT JOIN bs_attributes_added_info basa ";
          // $query = $query."    ON bpai.bs_product_id = basa.bs_product_id ";
          // $query = $query."      AND bpai.bs_attributes_id = basa.bs_attributes_id ";
          // $query = $query."LEFT JOIN gm_post_purchase gpp ";
          // $query = $query."  ON bpai.bs_product_attribute_id = gpp.gm_product_size_link_id ";
          // $query = $query."    AND gpp.product_color_id = 0 ";
          // $query = $query."    AND gpp.transaction_applied = '0' ";
          $query = $query."LEFT JOIN (SELECT gm_product_size_link_id AS bs_product_attribute_id, SUM(product_quantity) AS product_quantity ";
          $query = $query."           FROM gm_post_purchase ";
          $query = $query."           WHERE product_color_id = 0 AND transaction_applied = '0' ";
          $query = $query."           GROUP BY gm_product_size_link_id) gpp ";
          $query = $query."    ON bpai.bs_product_attribute_id = gpp.bs_product_attribute_id ";
          $query = $query."WHERE 1 = 1 ";
          $query = $query."  AND bpai.bs_product_id = ".$gmprodid." ";
          $query = $query."  AND (bpai.bs_product_attribute_quantity - IFNULL(gpp.product_quantity,0)) > 0 ";
          $query = $query."ORDER BY bas.bs_attributes_sort_order, bas.bs_attributes_id ";
        }         
        // echo "<br/>".$query."<br/>";

        $this->execute_query($query);

        if ($this->fok == 1)
        {
            $this->make_array($this->qresultset, false, false);
        }
        
        if ( isset($this->qresultset) )
        {
          @ $this->qresultset->free();
          unset($this->qresultset);
        }

        $this->qresultset = null;
        
    }

    public function get_product_order_info($prodid, $szid)
    {
      // echo "<br/>PRODID:".$prodid." SLID:".$slid."<br/>";
      $this->fok = 0;

      if ($prodid == 201 || $prodid == 202)
      {
        // '2-' + phi.product_description
        $query = "SELECT pp.product_name, chi.category_hl_name AS product_size_name, ";
        $query = $query." chi.category_hl_name AS product_size_description, phi.product_name AS product_color_name, ";
        $query = $query." pp.product_name AS product_information, 0 AS gm_product_size_link_id, ";
        $query = $query." 3 AS bs_display_style_id, 99 AS num_products, phi.product_info_id, pp.product_id, ";
        $query = $query." phi.product_suffix, cc.category_info_id, ";
        $query = $query." phi.product_small_image_file AS product_image_file, '' AS product_size_dimension, ";
        $query = $query." pp.product_tag AS color_tag, ";
        $query = $query." pp.product_price AS discount_price, pp.product_price AS unit_price, pp.product_description ";
        $query = $query." FROM category_hl_info chi ";
        $query = $query." INNER JOIN hl_category_product_link hcpl ";
        $query = $query."    ON chi.category_hl_info_id = hcpl.category_hl_info_id ";
        $query = $query."      AND chi.category_status = 0 ";
        $query = $query." INNER JOIN product_hl_info phi ";
        $query = $query."    ON hcpl.product_hl_info_id = phi.product_info_id ";
        $query = $query."      AND phi.product_status = 0 ";
        $query = $query." INNER JOIN gm_product2_info pp ";
        $query = $query."    ON pp.product_info_id = ".$prodid." ";
        $query = $query." INNER JOIN gm_category2_info cc ";
        $query = $query."    ON pp.category_info_id = cc.category_info_id ";
        $query = $query." WHERE phi.product_status = 0 ";
        $query = $query."   AND chi.category_hl_info_id = ".$szid." ";
        $query = $query." ORDER BY phi.product_sort_order ";
      }
      else if ($prodid == 61 || $prodid == 62 || $prodid == 63 || $prodid == 64 || $prodid == 65)
      {
        $query = "SELECT ";
        $query = $query."   bpi.bs_product_name, bas.bs_attributes_short_name, ";
        $query = $query."   CASE WHEN bas.bs_attributes_short_name = 'S' THEN 'Small' ";
        $query = $query."        WHEN bas.bs_attributes_short_name = 'M' THEN 'Medium' ";
        $query = $query."        WHEN bas.bs_attributes_short_name = 'L' THEN 'Large' ";
        $query = $query."        WHEN bas.bs_attributes_short_name = 'XL' THEN 'X-Large' ";
        $query = $query."        WHEN bas.bs_attributes_short_name = '2XL' THEN '2X-Large' ";
        $query = $query."        WHEN bas.bs_attributes_short_name = '3XL' THEN '3X-Large' ";
        $query = $query."        WHEN bas.bs_attributes_short_name = 'LS' THEN 'Ladies Small' ";
        $query = $query."        WHEN bas.bs_attributes_short_name = 'LM' THEN 'Ladies Medium' ";
        $query = $query."        WHEN bas.bs_attributes_short_name = 'LL' THEN 'Ladies Large' ";
        $query = $query."        WHEN bas.bs_attributes_short_name = 'LXL' THEN 'Ladies X-Large' ";
        $query = $query."        WHEN bas.bs_attributes_short_name = 'L2XL' THEN 'Ladies 2X-Large' ";
        $query = $query."        WHEN bas.bs_attributes_short_name = 'YL' THEN 'Youth Large' ";
        $query = $query."        ELSE bas.bs_attributes_short_name END AS attribute_name1, ";
        $query = $query."   CASE WHEN bpi.bs_product_cosz_code = 'C' THEN IFNULL(baca.bs_attributec_added_desc3, bac.bs_attributec_name) ";
        $query = $query."        WHEN bpi.bs_product_cosz_code = 'O' THEN IFNULL(bao.bs_attributeo_name, 'N/A') ";
        $query = $query."        ELSE IFNULL(baca.bs_attributec_added_desc3, bac.bs_attributec_name) END AS attribute_name2, ";
        $query = $query."   CASE WHEN bpi.bs_product_cosz_code = 'C' THEN bpi.bs_product_information ";
        $query = $query."        WHEN bpi.bs_product_cosz_code = 'O' THEN IFNULL(baoa.bs_attributeo_added_desc2, bpi.bs_product_information) ";
        $query = $query."        ELSE bpi.bs_product_information END AS product_information, ";
        $query = $query."   bpai.bs_product_attribute_id, bpi.bs_display_style_id, ";
        $query = $query."   (bpai.bs_product_attribute_quantity - IFNULL(gpp.product_quantity,0)) AS product_quantity, ";
        $query = $query."   bpi.bs_product_id, bpi.invoice_product_id, bpi.bs_product_tag, ";
        $query = $query."   bpi.bs_category_id, bpi.bs_product_sub_dir, ";
        $query = $query."   IFNULL(basa.bs_attributes_dimensions, '') AS dimensions, ";
        $query = $query."   CASE WHEN bpi.bs_product_cosz_code = 'C' THEN bac.bs_attributec_tag ";
        $query = $query."        WHEN bpi.bs_product_cosz_code = 'O' THEN bao.bs_attributeo_tag ";
        $query = $query."        ELSE bac.bs_attributec_tag END AS attribute_tag, ";
        $query = $query."   bpi.bs_product_price + IFNULL(basa.bs_attributes_surcharge, 0.0) + IFNULL(bao.bs_attributeo_surcharge, 0.0)  AS discount_unit_price, ";
        $query = $query."   bpi.bs_product_price + IFNULL(basa.bs_attributes_surcharge, 0.0) + IFNULL(bao.bs_attributeo_surcharge, 0.0)  AS unit_price, ";
        $query = $query."   IFNULL(baca.bs_attributec_added_desc1, bci.bs_category_description) AS bs_category_description, ";
        $query = $query."   bpi.bs_product_cosz_code, bpi.bs_product_category_code, basa.bs_attributes_image_count ";
        $query = $query."FROM bs_product_info bpi ";
        $query = $query."INNER JOIN bs_product_attribute_info bpai ";
        $query = $query."    ON bpi.bs_product_id = bpai.bs_product_id ";
        $query = $query."      AND bpi.bs_product_status = 1 ";
        $query = $query."      AND bpi.bs_product_id = ".$prodid." ";
        // $query = $query."      AND bpai.bs_attributes_id = ".$szid." ";
        $query = $query."      AND bpai.bs_product_attribute_status = 1 ";
        $query = $query."      AND bpai.bs_product_attribute_quantity > 0 ";
        $query = $query."INNER JOIN bs_attributes_info bas ";
        $query = $query."    ON bpai.bs_attributes_id = bas.bs_attributes_id ";
        $query = $query."      AND bas.bs_attributes_status = 1 ";
        $query = $query."      AND bas.bs_attributes_short_name = '".$szid."' ";
        $query = $query."INNER JOIN bs_attributes_added_info basa ";
        $query = $query."    ON bpai.bs_product_id = basa.bs_product_id ";
        $query = $query."      AND bpai.bs_attributes_id = basa.bs_attributes_id ";
        $query = $query."LEFT JOIN bs_attributec_info bac ";
        $query = $query."    ON bpai.bs_attributec_id = bac.bs_attributec_id ";
        $query = $query."LEFT JOIN bs_attributec_added_info baca ";
        $query = $query."    ON bpai.bs_product_id = baca.bs_product_id ";
        $query = $query."      AND bpai.bs_attributec_id = baca.bs_attributec_id ";
        $query = $query."LEFT JOIN bs_attributeo_info bao ";
        $query = $query."    ON bpai.bs_attributeo_id = bao.bs_attributeo_id ";
        $query = $query."LEFT JOIN  bs_attributeo_added_info baoa ";
        $query = $query."    ON bpai.bs_product_id = baoa.bs_product_id ";
        $query = $query."      AND bpai.bs_attributeo_id = baoa.bs_attributeo_id ";
        $query = $query."LEFT JOIN bs_category_info bci ";
        $query = $query."    ON bpi.bs_category_id = bci.bs_category_id ";
        $query = $query."LEFT JOIN (SELECT gm_product_size_link_id AS bs_product_attribute_id, SUM(product_quantity) AS product_quantity ";
        $query = $query."           FROM gm_post_purchase ";
        $query = $query."           WHERE product_color_id = 0 AND transaction_applied = '0' ";
        $query = $query."           GROUP BY gm_product_size_link_id) gpp ";
        $query = $query."    ON bpai.bs_product_attribute_id = gpp.bs_product_attribute_id ";
        $query = $query."WHERE 1 = 1 ";
        $query = $query."  AND (bpai.bs_product_attribute_quantity - IFNULL(gpp.product_quantity,0)) > 0 ";
        $query = $query."ORDER BY bas.bs_attributes_sort_order, bas.bs_attributes_id, ";
        $query = $query."    IFNULL(bac.bs_attributec_sort_order, bao.bs_attributeo_sort_order)  ";
      }
      else
      {
        // $query = $query."   CASE WHEN bpi.bs_product_cosz_code = 'C' THEN IFNULL(baca.bs_attributec_added_desc2, bpi.bs_product_information) ";
        $query = "SELECT ";
        $query = $query."   bpi.bs_product_name, bas.bs_attributes_short_name, ";
        $query = $query."   CASE WHEN bpi.bs_product_cosz_code = 'C' THEN bas.bs_attributes_name ";
        $query = $query."        WHEN bpi.bs_product_cosz_code = 'O' THEN IFNULL(baoa.bs_attributeo_added_desc3, bpi.bs_product_description) ";
        $query = $query."        ELSE bas.bs_attributes_name END AS attribute_name1, ";
        $query = $query."   CASE WHEN bpi.bs_product_cosz_code = 'C' THEN IFNULL(baca.bs_attributec_added_desc3, bac.bs_attributec_name) ";
        $query = $query."        WHEN bpi.bs_product_cosz_code = 'O' THEN IFNULL(bao.bs_attributeo_name, 'N/A') ";
        $query = $query."        ELSE IFNULL(baca.bs_attributec_added_desc3, bac.bs_attributec_name) END AS attribute_name2, ";
        $query = $query."   CASE WHEN bpi.bs_product_cosz_code = 'C' THEN bpi.bs_product_information ";
        $query = $query."        WHEN bpi.bs_product_cosz_code = 'O' THEN IFNULL(baoa.bs_attributeo_added_desc2, bpi.bs_product_information) ";
        $query = $query."        ELSE bpi.bs_product_information END AS product_information, ";
        $query = $query."   bpai.bs_product_attribute_id, bpi.bs_display_style_id, ";
        $query = $query."   (bpai.bs_product_attribute_quantity - IFNULL(gpp.product_quantity,0)) AS product_quantity, ";
        $query = $query."   bpi.bs_product_id, bpi.invoice_product_id, bpi.bs_product_tag, ";
        $query = $query."   bpi.bs_category_id, bpi.bs_product_sub_dir, ";
        $query = $query."   IFNULL(basa.bs_attributes_dimensions, '') AS dimensions, ";
        $query = $query."   CASE WHEN bpi.bs_product_cosz_code = 'C' THEN bac.bs_attributec_tag ";
        $query = $query."        WHEN bpi.bs_product_cosz_code = 'O' THEN bao.bs_attributeo_tag ";
        $query = $query."        ELSE bac.bs_attributec_tag END AS attribute_tag, ";
        $query = $query."   bpi.bs_product_price + IFNULL(basa.bs_attributes_surcharge, 0.0) + IFNULL(bao.bs_attributeo_surcharge, 0.0)  AS discount_unit_price, ";
        $query = $query."   bpi.bs_product_price + IFNULL(basa.bs_attributes_surcharge, 0.0) + IFNULL(bao.bs_attributeo_surcharge, 0.0)  AS unit_price, ";
        $query = $query."   IFNULL(baca.bs_attributec_added_desc1, bci.bs_category_description) AS bs_category_description, ";
        $query = $query."   bpi.bs_product_cosz_code, bpi.bs_product_category_code, basa.bs_attributes_image_count ";
        $query = $query."FROM bs_product_info bpi ";
        $query = $query."INNER JOIN bs_product_attribute_info bpai ";
        $query = $query."    ON bpi.bs_product_id = bpai.bs_product_id ";
        $query = $query."      AND bpi.bs_product_status = 1 ";
        $query = $query."      AND bpi.bs_product_id = ".$prodid." ";
        $query = $query."      AND bpai.bs_attributes_id = ".$szid." ";
        $query = $query."      AND bpai.bs_product_attribute_status = 1 ";
        $query = $query."      AND bpai.bs_product_attribute_quantity > 0 ";
        $query = $query."INNER JOIN bs_attributes_info bas ";
        $query = $query."    ON bpai.bs_attributes_id = bas.bs_attributes_id ";
        $query = $query."      AND bas.bs_attributes_status = 1 ";
        $query = $query."INNER JOIN bs_attributes_added_info basa ";
        $query = $query."    ON bpai.bs_product_id = basa.bs_product_id ";
        $query = $query."      AND bpai.bs_attributes_id = basa.bs_attributes_id ";
        $query = $query."LEFT JOIN bs_attributec_info bac ";
        $query = $query."    ON bpai.bs_attributec_id = bac.bs_attributec_id ";
        $query = $query."LEFT JOIN bs_attributec_added_info baca ";
        $query = $query."    ON bpai.bs_product_id = baca.bs_product_id ";
        $query = $query."      AND bpai.bs_attributec_id = baca.bs_attributec_id ";
        $query = $query."LEFT JOIN bs_attributeo_info bao ";
        $query = $query."    ON bpai.bs_attributeo_id = bao.bs_attributeo_id ";
        $query = $query."LEFT JOIN  bs_attributeo_added_info baoa ";
        $query = $query."    ON bpai.bs_product_id = baoa.bs_product_id ";
        $query = $query."      AND bpai.bs_attributeo_id = baoa.bs_attributeo_id ";
        $query = $query."LEFT JOIN bs_category_info bci ";
        $query = $query."    ON bpi.bs_category_id = bci.bs_category_id ";
        $query = $query."LEFT JOIN (SELECT gm_product_size_link_id AS bs_product_attribute_id, SUM(product_quantity) AS product_quantity ";
        $query = $query."           FROM gm_post_purchase ";
        $query = $query."           WHERE product_color_id = 0 AND transaction_applied = '0' ";
        $query = $query."           GROUP BY gm_product_size_link_id) gpp ";
        $query = $query."    ON bpai.bs_product_attribute_id = gpp.bs_product_attribute_id ";
        $query = $query."WHERE 1 = 1 ";
        $query = $query."  AND (bpai.bs_product_attribute_quantity - IFNULL(gpp.product_quantity,0)) > 0 ";
        $query = $query."ORDER BY bas.bs_attributes_sort_order, bas.bs_attributes_id, ";
        $query = $query."    IFNULL(bac.bs_attributec_sort_order, bao.bs_attributeo_sort_order)  ";
      }

/*     
      else if ($prodid == 7 || $prodid == 8 || $prodid == 15 || $prodid == 23 || $prodid == 24 || $prodid == 25 || $prodid == 39 || $prodid == 40 
               || $prodid == 53 || $prodid == 54)
      {
        $query = "SELECT ";
        $query = $query."   bpi.bs_product_name, bas.bs_attributes_short_name, bao.bs_attributeo_name, ";
        $query = $query."   IFNULL(baoa.bs_attributeo_added_desc3, bpi.bs_product_description) AS bs_product_description, ";
        $query = $query."   IFNULL(baoa.bs_attributeo_added_desc2, bpi.bs_product_information) AS bs_product_information, ";
        $query = $query."   bpai.bs_product_attribute_id, bpi.bs_display_style_id, ";
        $query = $query."   (bpai.bs_product_attribute_quantity - IFNULL(gpp.product_quantity,0)) AS product_quantity, ";
        $query = $query."   bpi.bs_product_id, bpi.invoice_product_id, bpi.bs_product_tag, ";
        $query = $query."   bpi.bs_category_id, bs_product_sub_dir, ";
        $query = $query."   basa.bs_attributes_dimensions, bao.bs_attributeo_tag, ";
        // $query = $query."   bpi.bs_product_price AS discount_price, ";
        // $query = $query."   bpi.bs_product_price AS product_price, ";
        // $query = $query."   basa.bs_attributes_surcharge, ";
        $query = $query."   bpi.bs_product_price + basa.bs_attributes_surcharge + IFNULL(bao.bs_attributeo_surcharge, 0.0) AS discount_unit_price, ";
        $query = $query."   bpi.bs_product_price + basa.bs_attributes_surcharge + IFNULL(bao.bs_attributeo_surcharge, 0.0)  AS unit_price, ";
        $query = $query."   IFNULL(baoa.bs_attributeo_added_desc1, bci.bs_category_description) AS bs_category_description ";
        $query = $query."FROM bs_product_info bpi ";
        $query = $query."INNER JOIN bs_product_attribute_info bpai ";
        $query = $query."    ON bpi.bs_product_id = ".$prodid." ";
        $query = $query."      AND bpi.bs_product_status = 1 ";
        $query = $query."      AND bpai.bs_attributes_id = ".$szid." ";
        $query = $query."      AND bpi.bs_product_id = bpai.bs_product_id ";
        $query = $query."      AND bpai.bs_product_attribute_status = 1 ";
        $query = $query."      AND bpai.bs_product_attribute_quantity > 0 ";
        $query = $query."INNER JOIN bs_attributes_info bas ";
        $query = $query."    ON bpai.bs_attributes_id = bas.bs_attributes_id ";
        $query = $query."      AND bas.bs_attributes_status = 1 ";
        $query = $query."INNER JOIN bs_attributes_added_info basa ";
        $query = $query."    ON bpai.bs_product_id = basa.bs_product_id ";
        $query = $query."      AND bpai.bs_attributes_id = basa.bs_attributes_id ";
        $query = $query."INNER JOIN bs_attributeo_info bao ";
        $query = $query."    ON bpai.bs_attributeo_id = bao.bs_attributeo_id ";
        $query = $query."INNER JOIN bs_category_info bci ";
        $query = $query."    ON bpi.bs_category_id = bci.bs_category_id ";
        $query = $query."LEFT JOIN bs_attributeo_added_info baoa ";
        $query = $query."    ON bpai.bs_product_id = baoa.bs_product_id ";
        $query = $query."      AND bpai.bs_attributeo_id = baoa.bs_attributeo_id ";
        $query = $query."LEFT JOIN bs_attributec_info bac ";
        $query = $query."    ON bpai.bs_attributec_id = bac.bs_attributec_id ";
        $query = $query."LEFT JOIN  bs_attributec_added_info baca ";
        $query = $query."    ON bpai.bs_product_id = baca.bs_product_id ";
        $query = $query."      AND bpai.bs_attributec_id = baca.bs_attributec_id ";
        $query = $query."LEFT JOIN (SELECT gm_product_size_link_id AS bs_product_attribute_id, SUM(product_quantity) AS product_quantity ";
        $query = $query."           FROM gm_post_purchase ";
        $query = $query."           WHERE product_color_id = 0 AND transaction_applied = '0' ";
        $query = $query."           GROUP BY gm_product_size_link_id) gpp ";
        $query = $query."    ON bpai.bs_product_attribute_id = gpp.bs_product_attribute_id ";
        $query = $query."WHERE 1 = 1 ";
        $query = $query."  AND (bpai.bs_product_attribute_quantity - IFNULL(gpp.product_quantity,0)) > 0 ";
        $query = $query."ORDER BY bao.bs_attributeo_sort_order, bao.bs_attributeo_id ";
      }
      else
      {
        $query = "SELECT ";
        $query = $query."   bpi.bs_product_name, bas.bs_attributes_short_name, bas.bs_attributes_name, ";
        $query = $query."   IFNULL(baca.bs_attributec_added_desc3, bac.bs_attributec_name) AS bs_attributec_name, ";
        $query = $query."   IFNULL(baca.bs_attributec_added_desc2, bpi.bs_product_information) AS bs_product_information, ";
        $query = $query."   bpai.bs_product_attribute_id, bpi.bs_display_style_id, ";
        $query = $query."   (bpai.bs_product_attribute_quantity - IFNULL(gpp.product_quantity,0)) AS product_quantity, ";
        $query = $query."   bpi.bs_product_id, bpi.invoice_product_id, bpi.bs_product_tag, ";
        $query = $query."   bpi.bs_category_id, bs_product_sub_dir, ";
        $query = $query."   IFNULL(basa.bs_attributes_dimensions, '') AS dimensions, bac.bs_attributec_tag, ";
        // $query = $query."   bpi.bs_product_price AS discount_price, ";
        // $query = $query."   bpi.bs_product_price AS product_price, ";
        // $query = $query."   basa.bs_attributes_surcharge, ";
        $query = $query."   bpi.bs_product_price + IFNULL(basa.bs_attributes_surcharge, 0.0) + IFNULL(bao.bs_attributeo_surcharge, 0.0)  AS discount_unit_price, ";
        $query = $query."   bpi.bs_product_price + IFNULL(basa.bs_attributes_surcharge, 0.0) + IFNULL(bao.bs_attributeo_surcharge, 0.0)  AS unit_price, ";
        $query = $query."   IFNULL(baca.bs_attributec_added_desc1, bci.bs_category_description) AS bs_category_description ";
        $query = $query."FROM bs_product_info bpi ";
        $query = $query."INNER JOIN bs_product_attribute_info bpai ";
        $query = $query."    ON bpi.bs_product_id = ".$prodid." ";
        $query = $query."      AND bpi.bs_product_status = 1 ";
        $query = $query."      AND bpai.bs_attributes_id = ".$szid." ";
        $query = $query."      AND bpi.bs_product_id = bpai.bs_product_id ";
        $query = $query."      AND bpai.bs_product_attribute_status = 1 ";
        $query = $query."      AND bpai.bs_product_attribute_quantity > 0 ";
        $query = $query."INNER JOIN bs_attributes_info bas ";
        $query = $query."    ON bpai.bs_attributes_id = bas.bs_attributes_id ";
        $query = $query."      AND bas.bs_attributes_status = 1 ";
        $query = $query."LEFT JOIN bs_attributes_added_info basa ";
        $query = $query."    ON bpai.bs_product_id = basa.bs_product_id ";
        $query = $query."      AND bpai.bs_attributes_id = basa.bs_attributes_id ";
        $query = $query."INNER JOIN bs_attributec_info bac ";
        $query = $query."    ON bpai.bs_attributec_id = bac.bs_attributec_id ";
        $query = $query."INNER JOIN bs_category_info bci ";
        $query = $query."    ON bpi.bs_category_id = bci.bs_category_id ";
        $query = $query."LEFT JOIN bs_attributec_added_info baca ";
        $query = $query."    ON bpai.bs_product_id = baca.bs_product_id ";
        $query = $query."      AND bpai.bs_attributec_id = baca.bs_attributec_id ";
        $query = $query."LEFT JOIN bs_attributeo_info bao ";
        $query = $query."    ON bpai.bs_attributeo_id = bao.bs_attributeo_id ";
        $query = $query."LEFT JOIN  bs_attributeo_added_info baoa ";
        $query = $query."    ON bpai.bs_product_id = baoa.bs_product_id ";
        $query = $query."      AND bpai.bs_attributeo_id = baoa.bs_attributeo_id ";
        $query = $query."LEFT JOIN (SELECT gm_product_size_link_id AS bs_product_attribute_id, SUM(product_quantity) AS product_quantity ";
        $query = $query."           FROM gm_post_purchase ";
        $query = $query."           WHERE product_color_id = 0 AND transaction_applied = '0' ";
        $query = $query."           GROUP BY gm_product_size_link_id) gpp ";
        $query = $query."    ON bpai.bs_product_attribute_id = gpp.bs_product_attribute_id ";
        $query = $query."WHERE 1 = 1 ";
        $query = $query."  AND (bpai.bs_product_attribute_quantity - IFNULL(gpp.product_quantity,0)) > 0 ";
        $query = $query."ORDER BY bas.bs_attributes_sort_order, bas.bs_attributes_id ";
      }
  */    
      // echo "<br/>".$query."<br/>";
      
      $this->execute_query($query);

      if ($this->fok == 1)
      {
        $this->make_array($this->qresultset, false, false);
      }
        
      if ( isset($this->qresultset) )
      {
        @ $this->qresultset->free();
        unset($this->qresultset);
      }

      $this->qresultset = null;
        
    }

    public function OBS_get_category_list()
    {
        $this->fok = 0;
        $query = "SELECT category_info_id, category_name, category_sort_order, category_description, category_button_name ";
        $query = $query."FROM gm_category2_info ";
        $query = $query."WHERE category_status = 1 ";
        $query = $query."  AND category_info_id < 200 ";
        $query = $query."ORDER BY category_sort_order, category_name; ";

        $this->execute_query($query);

        if ($this->fok == 1)
        {
            $this->make_array($this->qresultset, false, false);
        }
        
        if ( isset($this->qresultset) )
        {
          $this->qresultset->free();
          unset($this->qresultset);
        }

        $this->qresultset = null;
        
    }

    public function OBS_get_sizebutton_list($gmprodid)
    {
        $this->fok = 0;

        if ($gmprodid == 201)
        {
          // Heavenly Lips
          $strWhere = " AND chi.category_hl_info_id < 12 ";
          $query = "SELECT DISTINCT ";
          $query = $query."   chi.category_hl_name AS product_size_name, chi.category_hl_name AS product_size_description, ";
          $query = $query."  chi.category_hl_info_id, chi.category_sort_order ";
          $query = $query."FROM category_hl_info chi ";
          $query = $query."INNER JOIN hl_category_product_link hcpl "; 
          $query = $query."  ON chi.category_hl_info_id = hcpl.category_hl_info_id "; 
          $query = $query."    AND chi.category_status = 0 "; 
          $query = $query."    AND chi.category_hl_info_id <> 0 ";
          $query = $query."INNER JOIN product_hl_info phi "; 
          $query = $query."  ON hcpl.product_hl_info_id = phi.product_info_id "; 
          $query = $query."    AND phi.product_status = 0 ";
          $query = $query."WHERE 1 = 1 ".$strWhere; // No Pencils
          $query = $query."ORDER BY chi.category_sort_order ";
        }
        else if ($gmprodid == 202)
        {
          // Heavenly Pencils
          $strWhere = " AND chi.category_hl_info_id = 12 ";
          $query = "SELECT DISTINCT ";
          $query = $query."   chi.category_hl_name AS product_size_name, chi.category_hl_name AS product_size_description, ";
          $query = $query."  chi.category_hl_info_id, chi.category_sort_order ";
          $query = $query."FROM category_hl_info chi ";
          $query = $query."INNER JOIN hl_category_product_link hcpl "; 
          $query = $query."  ON chi.category_hl_info_id = hcpl.category_hl_info_id "; 
          $query = $query."    AND chi.category_status = 0 "; 
          $query = $query."    AND chi.category_hl_info_id <> 0 ";
          $query = $query."INNER JOIN product_hl_info phi "; 
          $query = $query."  ON hcpl.product_hl_info_id = phi.product_info_id "; 
          $query = $query."    AND phi.product_status = 0 ";
          $query = $query."WHERE 1 = 1 ".$strWhere; // No Pencils
          $query = $query."ORDER BY chi.category_sort_order ";
        }
        else
        {
          // default to GM
          $strWhere = " ";
          $query = "SELECT DISTINCT ";
          $query = $query."  CASE WHEN c.product_size_name = 'N/A' THEN b.product_button_name ELSE c.product_size_name END AS product_size_name, ";
          $query = $query."  CASE WHEN c.product_size_description = 'N/A' THEN b.product_button_name ";
          $query = $query."    WHEN b.category_info_id = 6 THEN a.product_size_description ";
          $query = $query."    ELSE c.product_size_description END AS product_size_description, ";
          $query = $query."  a.gm_product_size_link_id, c.product_size_sort_order ";
          $query = $query."FROM gm_product_size_link a ";
          $query = $query."INNER JOIN gm_product2_info b ";
          $query = $query."  ON a.product_info_id = b.product_info_id ";
          $query = $query."INNER JOIN product_size c ";
          $query = $query."  ON a.product_size_id = c.product_size_id ";
          $query = $query."    AND c.product_size_status = 1 ";
          $query = $query."INNER JOIN gm_product_size_color_link d ";
          $query = $query."  ON a.gm_product_size_link_id = d.gm_product_size_link_id ";
          $query = $query."    AND d.product_quantity > 0 ";
          $query = $query."    AND d.status = 1 ";
          $query = $query."LEFT JOIN gm_post_purchase gpp ";
          $query = $query."  ON d.gm_product_size_link_id = gpp.gm_product_size_link_id ";
          $query = $query."    AND d.product_color_id = gpp.product_color_id ";
          $query = $query."    AND gpp.transaction_applied = '0' ";
          $query = $query."WHERE b.product_info_id = ".$gmprodid." ";
          $query = $query."  AND b.product_status >= 1 ";
          $query = $query."  AND (d.product_quantity - IFNULL(gpp.product_quantity,0)) > 0 ";
          $query = $query."ORDER BY c.product_size_sort_order, gm_product_size_link_id ";
        }

        //echo "<br/>".$query."<br/>";

        $this->execute_query($query);

        if ($this->fok == 1)
        {
            $this->make_array($this->qresultset, false, false);
        }
        
        if ( isset($this->qresultset) )
        {
          @ $this->qresultset->free();
          unset($this->qresultset);
        }

        $this->qresultset = null;
        
    }

    public function OBS_get_product_list($CategoryID)
    {
        $this->fok = 0;

        if ($CategoryID >= 100 && $CategoryID < 200)
        {
            $query = "SELECT DISTINCT b.product_info_id, b.product_name, b.product_sort_order ";
            $query = $query."FROM gm_product2_info b ";
            $query = $query."WHERE b.category_info_id = ".$CategoryID." ";
            $query = $query."  AND b.product_status >= 1 ";
    //        $query = $query."  AND (d.product_quantity - IFNULL(gpp.product_quantity,0)) > 0 ";
            $query = $query."ORDER BY b.product_sort_order, b.product_name; ";
        }
        else
        {
            $query = "SELECT DISTINCT b.product_info_id, b.product_name, b.product_sort_order ";
            $query = $query."FROM gm_product_size_link a ";
            $query = $query."INNER JOIN gm_product2_info b ";
            $query = $query."  ON a.product_info_id = b.product_info_id "; // AND a.product_info_id <> 46 ";
            $query = $query."INNER JOIN product_size c ";
            $query = $query."  ON a.product_size_id = c.product_size_id ";
            $query = $query."    AND c.product_size_status = 1 ";
            $query = $query."INNER JOIN gm_product_size_color_link d ";
            $query = $query."  ON a.gm_product_size_link_id = d.gm_product_size_link_id ";
            $query = $query."    AND d.product_quantity > 0 ";
            $query = $query."    AND d.status = 1 ";
            $query = $query."LEFT JOIN gm_post_purchase gpp ";
            $query = $query."  ON d.gm_product_size_link_id = gpp.gm_product_size_link_id ";
            $query = $query."    AND d.product_color_id = gpp.product_color_id ";
            $query = $query."    AND gpp.transaction_applied = '0' ";
            $query = $query."WHERE b.category_info_id = ".$CategoryID." ";
            $query = $query."  AND b.product_status >= 1 ";
            $query = $query."  AND (d.product_quantity - IFNULL(gpp.product_quantity,0)) > 0 ";
            $query = $query."ORDER BY b.product_sort_order, b.product_name; ";
        }

        // echo "<br/>".$query."<br/>";
        $this->execute_query($query);

        if ($this->fok == 1)
        {
            $this->make_array($this->qresultset, false, false);
        }
        
        if ( isset($this->qresultset) )
        {
          @ $this->qresultset->free();
          unset($this->qresultset);
        }

        $this->qresultset = null;
        
    }

    public function OBS_get_product_order_info($prodid, $slid)
    {
      // echo "<br/>PRODID:".$prodid." SLID:".$slid."<br/>";
      $this->fok = 0;

      if ($prodid == 201 || $prodid == 202)
      {
        $query = "SELECT pp.product_name, chi.category_hl_name AS product_size_name, ";
        $query = $query." chi.category_hl_name AS product_size_description, 1 AS snsid, phi.product_name AS product_color_name, ";
        $query = $query." 'nop1' AS filler_nop1, phi.product_description AS product_information, 1 AS gm_product_size_link_id, ";
        $query = $query." 2 AS product_color_id, 99 AS num_products, phi.product_info_id, pp.product_id, ";
        $query = $query." phi.product_suffix, cc.category_info_id, ";
        $query = $query." phi.product_small_image_file AS product_image_file, '' AS product_size_dimension, ";
        $query = $query." pp.product_tag AS color_tag, pp.product_price AS discount_price, pp.product_price, ";
        $query = $query." '0.00' AS product_size_surcharge, pp.product_price AS discount_price, pp.product_price AS unit_price ";
        $query = $query." FROM category_hl_info chi ";
        $query = $query." INNER JOIN hl_category_product_link hcpl ";
        $query = $query."    ON chi.category_hl_info_id = hcpl.category_hl_info_id ";
        $query = $query."      AND chi.category_status = 0 ";
        $query = $query." INNER JOIN product_hl_info phi ";
        $query = $query."    ON hcpl.product_hl_info_id = phi.product_info_id ";
        $query = $query."      AND phi.product_status = 0 ";
        $query = $query." INNER JOIN gm_product2_info pp ";
        $query = $query."    ON pp.product_info_id = ".$prodid." ";
        $query = $query." INNER JOIN gm_category2_info cc ";
        $query = $query."    ON pp.category_info_id = cc.category_info_id ";
        $query = $query." WHERE phi.product_status = 0 ";
        $query = $query."   AND chi.category_hl_info_id = ".$slid." ";
        $query = $query." ORDER BY phi.product_sort_order ";
      }
      else
      {
        // GM
        $query = "SELECT b.product_name, c.product_size_name, a.product_size_description, b.snsid, ";
        $query = $query."   e.product_color_name, 'NOP' AS filler_nop2, b.product_information, ";
        $query = $query."   d.gm_product_size_link_id, d.product_color_id, ";
        $query = $query."   SUM(d.product_quantity - IFNULL(gpp.product_quantity,0)) AS num_products, ";
        $query = $query."   b.product_info_id, b.product_id, ";
        $query = $query."   CASE WHEN b.product_status = 2 THEN c.product_size_name "; 
        $query = $query."        ELSE b.product_tag END AS product_tag, ";
        $query = $query."   b.category_info_id, ";
        $query = $query."   b.product_tag + '_' + e.color_tag + '.jpg' AS product_image_file, ";
        $query = $query."   a.product_size_dimension, e.color_tag, ";
        $query = $query."   CASE WHEN f.discount_price IS NOT NULL THEN f.discount_price ";
        $query = $query."        ELSE CASE WHEN b.product_status = 2 THEN c.product_size_surcharge ";
        $query = $query."                  ELSE b.product_price END   END AS discount_price, ";
        $query = $query."   CASE WHEN b.product_status = 2 THEN c.product_size_surcharge ";
        $query = $query."        ELSE b.product_price END AS product_price, ";
        $query = $query."   CASE WHEN b.product_status = 2 THEN 0.0 ";
        $query = $query."        ELSE a.product_size_surcharge END AS product_size_surcharge, ";
        $query = $query."   CASE WHEN f.discount_price IS NOT NULL THEN f.discount_price + a.product_size_surcharge ";
        $query = $query."        ELSE CASE WHEN b.product_status = 2 THEN c.product_size_surcharge ";
        $query = $query."                  ELSE b.product_price + a.product_size_surcharge END   END AS discount_unit_price, ";
        $query = $query."   CASE WHEN b.product_status = 2 THEN c.product_size_surcharge ";
        $query = $query."        ELSE b.product_price + a.product_size_surcharge END AS unit_price ";
        $query = $query."   , ci.category_description ";
        $query = $query."FROM gm_product_size_link a ";
        $query = $query."INNER JOIN gm_product2_info b ";
        $query = $query."  ON a.product_info_id = b.product_info_id ";
        $query = $query."    AND a.product_info_id = ".$prodid." ";
        $query = $query."    AND a.gm_product_size_link_id = ".$slid." ";
        $query = $query."INNER JOIN gm_category2_info ci ";
        $query = $query."  ON b.category_info_id = ci.category_info_id ";
        $query = $query."INNER JOIN product_size c ";
        $query = $query."  ON a.product_size_id = c.product_size_id ";
        $query = $query."INNER JOIN gm_product_size_color_link d ";
        $query = $query."  ON a.gm_product_size_link_id = d.gm_product_size_link_id ";
        $query = $query."    AND d.status = 1 ";
        $query = $query."INNER JOIN product_color e ";
        $query = $query."  ON d.product_color_id = e.product_color_id ";
        $query = $query."LEFT JOIN gm_post_purchase gpp ";
        $query = $query."  ON d.gm_product_size_link_id = gpp.gm_product_size_link_id ";
        $query = $query."    AND d.product_color_id = gpp.product_color_id ";
        $query = $query."    AND gpp.transaction_applied = '0' ";
        $query = $query."LEFT JOIN gm_discount_info f ";
        $query = $query."  ON b.product_info_id = f.product_info_id ";
        $query = $query."    AND CURDATE() BETWEEN f.discount_start_date AND f.discount_end_date ";
        $query = $query."WHERE 1 = 1 ";
        $query = $query."GROUP BY b.product_info_id, b.product_name, c.product_size_name, ";
        $query = $query."   c.product_size_description, e.product_color_name, e.image_file, d.gm_product_size_link_id, d.product_color_id ";
        $query = $query."HAVING SUM(d.product_quantity - IFNULL(gpp.product_quantity,0)) > 0 ";
        $query = $query."ORDER BY c.product_size_sort_order, b.product_sort_order, e.product_color_sort_order, e.product_color_name ";
      }

      // echo "<br/>".$query."<br/>";
      
      $this->execute_query($query);

      if ($this->fok == 1)
      {
        $this->make_array($this->qresultset, false, false);
      }
        
      if ( isset($this->qresultset) )
      {
        @ $this->qresultset->free();
        unset($this->qresultset);
      }

      $this->qresultset = null;
        
    }

    public function OBS_get_product_info($prodid)
    {
        $this->fok = 0;
        $query = "SELECT a.category_image_file, b.product_information ";
        $query = $query."FROM gm_category2_info a ";
        $query = $query."INNER JOIN gm_product2_info b ";
        $query = $query."  ON a.category_info_id = b.category_info_id ";
        $query = $query."WHERE b.product_info_id = ".$prodid." ";

        $query = "SELECT b.product_image_file, b.product_information ";
        $query = $query."FROM gm_product2_info b ";
        $query = $query."WHERE b.product_info_id = ".$prodid." ";

        // echo "<br/>".$query."<br/>";
        
        $this->execute_query($query);

        if ($this->fok == 1)
        {
            $this->make_array($this->qresultset, false, false);
        }
        
        if ( isset($this->qresultset) )
        {
          @ $this->qresultset->free();
          unset($this->qresultset);
        }

        $this->qresultset = null;
        
    }

  private function execute_query($thequery)
  {
    $this->rs_numcols = -1;
    $this->rs_numrows = -1;
    $this->numrows = $this->rs_numrows;
    $this->numcols = $this->rs_numcols;
    $this->fok = 0;

    if ( $this->dbg->debug == 1 )
    {
      echo "QUERY:".$thequery."<br/>";
    }

    $this->qresultset = $this->da1->conn->query($thequery);

    if ($this->qresultset)
    {
        $this->rs_numcols = $this->qresultset->field_count;
        $this->rs_numrows = $this->qresultset->num_rows;
        
        if ($this->rs_numrows > 0)
        {
          $this->fok = 1;
        }
        else
        {
          $this->fok = 0;
        }
    }
    else
    {
        $this->LastError = $this->da1->conn->error;
    }
    
    $this->numrows = $this->rs_numrows;
    $this->numcols = $this->rs_numcols;

  }


  private function make_array($rs, $addHeaders = true, $revData = false)
  {
    $row = 0;
    $numrows = $this->rs_numrows;
    $numcols = $this->rs_numcols;
    $this->numrows = $numrows; // Add 1 due to Header row

    $this->numcols = $numcols;

    if ($addHeaders == true)
    {
      $this->numrows = $numrows+1; // Add 1 due to Header row
      
      for ($j=0; $j<$numcols; $j++)
      {
        $fld_info = $rs->fetch_field_direct($j);
        $this->aresultset[$row][$j] = $fld_info->name;
      }
      $row = 1;
    }
    else
    {
      $row = 0;
    }
    
    for ($r=0; $r < $numrows; $r++)
    {
      $rr = $rs->fetch_array();

      for($c=0; $c < $numcols; $c++)
      {
        $this->aresultset[$row][$c] = $rr[$c];
      }

      $row++;
    }

    if ($revData == true)
    {
      $r1 = 1;
      $r2 = $numrows;  
      
      while ($r1 < $r2)
      {
        $rHold = $this->aresultset[$r1];
        $this->aresultset[$r1] = $this->aresultset[$r2];
        $this->aresultset[$r2] = $rHold;
        
        $r1++;
        $r2--;
        
      }

      $row++;
    }
  }

  private function BuildQueryWithGDG($q)
  {
    $sRtn = "";
    $GDG = explode(" ",$this->TGDGIDS);
    $numgdgids = count($GDG);
        
    if ($numgdgids == 1)
    {
      $sRtn = str_replace("<TABLEGDGID>",$GDG[0],$q);
    }
    else
    {
      $ndx = 0; 
           
      while ($ndx < $numgdgids)
      {
        $sRtn = $sRtn."(".str_replace("<TABLEGDGID>",$GDG[$ndx],$q).")";
        $ndx++;
              
        if ($ndx < $numgdgids)
        {
          $sRtn = $sRtn." UNION ";
        }
      }
    }
        
    return $sRtn;
            
  }
    
    private function OBS_get_log_messages_for_orderguid($TheGuid)
    {
        $this->fok = 0;
        $OrderBy = "ORDER BY li.log_info_id ";
        $MaxPages = "LIMIT 500 ";

        $query = "SELECT li.log_info_id AS LogID, li.log_datetime AS LogTS, li.log_type AS LogType, ";
        $query = $query." CONCAT(li.parent_function_name,'.',li.function_name) AS FunctionName, ";
        $query = $query." li.program_location AS ProgLoc, li.message_type AS MsgType, ";
        $query = $query." lmi.log_message AS Message ";
        $query = $query."FROM log_info_ li INNER JOIN log_message_info_100004 lmi ON li.log_info_id = lmi.log_info_id ";
        $query = $query."WHERE 1 = 1 ";
        $query = $query."  AND li.guid1 = '".$TheGuid."' ";
        $query = $query.$OrderBy;
        $query = $query.$MaxPages;

        $this->execute_query($query);

        if ($this->fok == 1)
        {
            $this->make_array($this->qresultset, true, false);
        }

        $this->qresultset->free();
        unset($this->qresultset);

    }

    private function OBS_get_log_startendid_info_BACKUP($SearchDate, $SearchFromTime, $SearchThruTime, $SearchLogType, $SearchFunctionName, $TheGuid, $TheVar, $FullWord)
    {
        $this->fok = 0;
        $FunctionNameSQL = $SearchFunctionName;
        $LogTypeSQL = "  AND li.message_type = '".$SearchLogType."' ";

        if (strlen($TheGuid) > 0)
        {
            $GuidSQL = " AND li.guid1 = '".$TheGuid."' ";
        }
        else
        {
            $GuidSQL = " ";
        }

        if (strlen($TheVar) > 0)
        {
            $VariSQL = "INNER JOIN log_message_info_100004 lmi ON li.log_info_id = lmi.log_info_id ";

            if ($FullWord == "1")
            {
              $VariSQL = $VariSQL." AND MATCH(lmi.log_message) AGAINST ('".$TheVar."') ";
            }
            else
            {
              $VariSQL = $VariSQL." AND lmi.log_message LIKE '%".$TheVar."%' ";
            }              
            
            $GuidSQL = "";
        }
        else
        {
            $VariSQL = "";
        }

        if (strtolower($SearchLogType) == "all")
        {
            $LogTypeSQL = " ";
        }

        if (strtolower($SearchFunctionName) == "all")
        {
            $FunctionNameSQL = " ";
        }
        else 
        {
            $pos = strpos($SearchFunctionName, ".");
            
            if ($pos > 0)
            {
                $FunctionNameSQL = " AND li.parent_function_name = '".substr($SearchFunctionName, 0, $pos)."' ";
                $FunctionNameSQL = $FunctionNameSQL." AND li.function_name = '".substr($SearchFunctionName, $pos + 1)."' ";
            }
            else
            {
                $FunctionNameSQL = " AND li.function_name LIKE '".$SearchFunctionName."%' ";
            }
            
        }

        $query = "SELECT MIN(li.log_info_id) AS MINLogID, MAX(li.log_info_id) AS MAXLogID, COUNT(*) AS NumRecs ";
        $query = $query."FROM log_info_100004 li ";
        $query = $query.$VariSQL;
        $query = $query."WHERE 1 = 1 ";
        $query = $query."  AND li.log_datetime BETWEEN '".$SearchDate." ".$SearchFromTime."' AND '".$SearchDate." ".$SearchThruTime."' ";
        $query = $query.$LogTypeSQL.$FunctionNameSQL.$GuidSQL;
        $query = $query."HAVING COUNT(*) > 0 ";
        
        $this->execute_query($query);

        if ($this->fok == 1)
        {
            $this->make_array($this->qresultset, false);
        }
        else
        {
            $this->aresultset = NULL;
        }

        $this->qresultset->free();
        unset($this->qresultset);

    }

    private function OBS_get_simple_simon($docsearch)
    {
        $this->fok = 0;

        $query = "SELECT ssid, ss_name, ss_description, ss_other_desc, create_date, create_user ";
        $query = $query."FROM simple_simon ";
        $query = $query."WHERE 1 = 1 ";

        if (strlen($docsearch) > 0)
        {
            $query = $query."AND ss_description LIKE '%".$docsearch."%' ";
        }

        $query = $query."ORDER BY ssid ";

        $this->execute_query($query);

        if ($this->fok == 1)
        {
            $this->make_array($this->qresultset);
        }

        $this->qresultset->free();
        unset($this->qresultset);

    }


}
?>
    
