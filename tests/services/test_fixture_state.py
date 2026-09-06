import struct
import unittest
from fixture_state import prepare, crc

class DiagnosticStateTest(unittest.TestCase):
    def test_species_guard_ignores_packed_tera_type(self):
        mon=bytearray(100)
        struct.pack_into('<H',mon,32,25 | (14 << 11))
        struct.pack_into('<H',mon,28,25 | (14 << 11))
        struct.pack_into('<H',mon,88,19)
        trainer=bytearray(48)
        struct.pack_into('<IHH',trainer,0,0x53435931,1,48)
        struct.pack_into('<H',trainer,30,crc(trainer))
        result=prepare(dict(party_address='0x02000000',party_hex=mon.hex(),trainer_address='0x02001000',trainer_hex=trainer.hex()))
        self.assertEqual(struct.unpack_from('<H',bytes.fromhex(result['party_after']),86)[0],16)
        self.assertTrue(result['changes'])
