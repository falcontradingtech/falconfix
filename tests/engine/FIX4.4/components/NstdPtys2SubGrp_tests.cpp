// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: NstdPtys2SubGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/NstdPtys2SubGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class NstdPtys2SubGrpComponentTest : public ::testing::Test {
protected:
    NstdPtys2SubGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(NstdPtys2SubGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setNested2PartySubIDType(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(NstdPtys2SubGrpComponentTest, SetNested2PartySubIDTypeAndNested2PartySubIDTypeMatch) {
    const int64_t test_value = 12345;
    component.setNested2PartySubIDType(test_value);
    EXPECT_EQ(component.getNested2PartySubIDType(), test_value);
    EXPECT_TRUE(component.hasNested2PartySubIDType());
}

TEST_F(NstdPtys2SubGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setNested2PartySubIDType(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(NstdPtys2SubGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[124];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    NstdPtys2SubGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(NstdPtys2SubGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
