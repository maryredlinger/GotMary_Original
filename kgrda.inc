<?php
class kgr_dataaccess
{
    public $conn;
    public $RWECONN;
    
    private $dbtype = "MYSQL";
    private $servername = "ECS24";
    private $database = "sunpro";
    private $username = "web_reader";
    private $password = "reader_web";

    // MySQL 5.0
    private $my_dbhostname = "sunpro613.db.12100650.hostedresource.com";
    private $my_dbname = "sunpro613";
    private $my_dbusername = "webdude613";
    private $my_dbpassword = "edudBEW!2";
    private $my_dbport = "3306";
    
    // MySQL 5.5
    private $my_dbhostname_428 = "db.cloudmerchant.net"; // "sunpro615.db.12100650.hostedresource.com";
    private $my_dbname_428 = "stt0428";
    private $my_dbusername_428 = "webdude615";
    private $my_dbpassword_428 = "edudBEW!2";
    private $my_dbport_428 = "3306";

    // MySQL 5.5
    private $my_dbhostname_615 = "db.cloudmerchant.net"; // "sunpro615.db.12100650.hostedresource.com";
    private $my_dbname_615 = "sunpro615";
    private $my_dbusername_615 = "webdude615";
    private $my_dbpassword_615 = "edudBEW!2";
    private $my_dbport_615 = "3306";

    // MySQL 5.6
    private $my_dbhostname_hg = "db.kgrdata.com"; 
    private $my_dbname_hg = "kgr513_spgm";
    private $my_dbusername_hg = "kgr513_webdude";
    private $my_dbpassword_hg = "edudBEW!2";
    private $my_dbport_hg = "3306";

    function __construct($thetype, $thedb)
    {
        $this->RWECONN = 0;
       
        if ($thedb == "sphg")
        {
            $this->servername = $this->my_dbhostname_hg;
            $this->database = $this->my_dbname_hg;
            $this->username = $this->my_dbusername_hg;
            $this->password = $this->my_dbpassword_hg;
        }
        else if ($thedb == "sp615")
        {
            $this->servername = $this->my_dbhostname_615;
            $this->database = $this->my_dbname_615;
            $this->username = $this->my_dbusername_615;
            $this->password = $this->my_dbpassword_615;
        }
	      else if ($thedb == "stt428")
        {
            $this->servername = $this->my_dbhostname_428;
            $this->database = $this->my_dbname_428;
            $this->username = $this->my_dbusername_428;
            $this->password = $this->my_dbpassword_428;
        }
        else if ($thedb == "sp613")
        {
            $this->servername = $this->my_dbhostname;
            $this->database = $this->my_dbname;
            $this->username = $this->my_dbusername;
            $this->password = $this->my_dbpassword;
        }
        else if ($thedb == "sp")
        {
            $this->database = $this->sp_database;
        }
        else if ($thedb == "fdb")
        {
            $this->database = $this->fdb_database;
        }
        
        // For now we can only access MSSQL DB
        if ($thetype == 'MYSQL')
        {
            $this->my_connect_db();
        }
        else
        {
            $this->dbtype = $thetype;
        }
    }
    
    function __destruct()
    {
        if ($this->RWECONN == 1)
        {
            if ($thetype == 'MYSQL')
            {
                $this->my_disconnect_db();
            }
        }
        
        $this->RWECONN = 0;
        $this->conn = null;
    }
  
    private function my_connect_db()
    {
     // Open the Connection
      $this->RWECONN = 0;
      $this->conn = new mysqli($this->servername, $this->username, $this->password, $this->database);

      if ($this->conn->connect_error)
      {
        die("Connection Failed: ".$this->conn->connect_error);
            echo "RESPONSE:SC1209:".$this->conn;
            echo "</body></html>";
      }
      else
      {
        $this->RWECONN = 1;
      }

    }

    private function disconnect_db()
    {
        //close the connection
//        mssql_close($this->dbhandle);

        odbc_close($this->conn);
    }                  
    
    private function my_disconnect_db()
    {

        @ $this->conn->close();    
    
    }                  

}
?>
