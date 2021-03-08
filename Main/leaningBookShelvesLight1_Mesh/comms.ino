/*----------------------------comms----------------------------*/
//void comms() { }

void setupMesh() 
{
  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  //mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  //mesh.setDebugMsgTypes( CONNECTION | SYNC );
  
  // There is an issue with ESP32 crashing. It seesm to work if the following debug options are left active.
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION | SYNC  );

  //mesh.init( MESH_NAME, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, STATION_CHANNEL );
  mesh.init(MESH_NAME, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, STATION_CHANNEL, MESH_NODE_HIDDEN, MESH_NODE_MAX_CONN); // hidden

  mesh.setContainsRoot(true);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
}
