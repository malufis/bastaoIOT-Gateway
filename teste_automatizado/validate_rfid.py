def decode_wl134(frame):
    if len(frame) != 30:
        return None, "Frame length must be 30"
    
    if frame[0] != 0x02 or frame[29] != 0x03:
        return None, "Invalid STX or ETX"
        
    # Checksum validation
    checksum = 0
    for i in range(1, 27):
        checksum ^= frame[i]
        
    if checksum != frame[27]:
        return None, f"Checksum mismatch: calculated {checksum:02X}, got {frame[27]:02X}"
        
    inverted_checksum = (~checksum) & 0xFF
    if inverted_checksum != frame[28]:
        return None, f"Inverted checksum mismatch: calculated {inverted_checksum:02X}, got {frame[28]:02X}"
        
    # Extract fields
    card_hex_lsb = "".join(chr(x) for x in frame[1:11])
    country_hex_lsb = "".join(chr(x) for x in frame[11:15])
    
    # Reverse to MSB-first
    card_hex_msb = card_hex_lsb[::-1]
    country_hex_msb = country_hex_lsb[::-1]
    
    # Convert to decimal
    card_dec = int(card_hex_msb, 16)
    country_dec = int(country_hex_msb, 16)
    
    return f"{country_dec:03d}{card_dec:012d}", None

def decode_yrm100(frame):
    if len(frame) < 7:
        return None, "Frame too short"
    if frame[0] != 0xBB or frame[-1] != 0x7E:
        return None, "Invalid Header or End"
        
    pl = (frame[3] << 8) | frame[4]
    if len(frame) != 7 + pl:
        return None, f"Length mismatch: pl={pl}, frame len={len(frame)}"
        
    # Checksum validation (Type up to Parameter)
    checksum = sum(frame[1:5+pl]) % 256
    if checksum != frame[5+pl]:
        return None, f"Checksum mismatch: calculated {checksum:02X}, got {frame[5+pl]:02X}"
        
    cmd = frame[2]
    if cmd == 0x22: # Single Inventory
        # Params starts at index 5. Length is PL.
        # Format: RSSI (1 byte), PC (2 bytes), EPC (PL - 5 bytes), CRC (2 bytes)
        epc_bytes = frame[8:8 + (pl - 5)]
        epc_hex = "".join(f"{x:02X}" for x in epc_bytes)
        return epc_hex, None
        
    return None, f"Unknown command {cmd:02X}"

# Run tests
if __name__ == "__main__":
    # Test WL-134
    # Frame bytes based on rfid_protocols.md:
    wl_frame = [0x02] + [ord(c) for c in "171A9253A3"] + [ord(c) for c in "4830"] + [ord("0"), ord("1")] + [ord("0")]*10 + [0x07, 0xF8, 0x03]
    tag, err = decode_wl134(wl_frame)
    print(f"WL-134 Decoded: {tag} (Error: {err})")
    assert tag == "900250000023921", "WL-134 test failed"
    
    # Test YRM100
    yrm_frame = [0xBB, 0x02, 0x22, 0x00, 0x11, 0xC9, 0x34, 0x00, 0x30, 0x75, 0x1F, 0xEB, 0x70, 0x5C, 0x59, 0x04, 0xE3, 0xD5, 0x0D, 0x70, 0x3A, 0x76, 0xEF, 0x7E]
    epc, err = decode_yrm100(yrm_frame)
    print(f"YRM100 EPC Decoded: {epc} (Error: {err})")
    assert epc == "30751FEB705C5904E3D50D70", "YRM100 test failed"
    print("All tests passed!")
