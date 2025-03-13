const uint32_t SECRET_KEY = 0xAAAAAAAA;  // Keep this secret

String getDeviceId() {
  uint64_t chipId = ESP.getEfuseMac();
  char deviceId[13];
  sprintf(deviceId, "%04X%08X", (uint16_t)(chipId >> 32), (uint32_t)chipId);
  return String(deviceId);
}

uint32_t hash(const String& str) {
  uint32_t hash = 5381;
  for (char c : str) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }
  return hash;
}

uint32_t generateSignature() {
  String deviceId = getDeviceId();
  uint32_t deviceHash = hash(deviceId);
  return deviceHash ^ SECRET_KEY;
}
