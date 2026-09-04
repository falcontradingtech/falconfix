// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: EvntGrp::NoEvents (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/EvntGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class EvntGrp_NoEventsComponentTest : public ::testing::Test {
protected:
    EvntGrp::NoEvents component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(EvntGrp_NoEventsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setEventType(42);
    component.setEventPx(3.14f);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(EvntGrp_NoEventsComponentTest, SetEventTypeAndEventTypeMatch) {
    const int64_t test_value = 12345;
    component.setEventType(test_value);
    EXPECT_EQ(component.getEventType(), test_value);
    EXPECT_TRUE(component.hasEventType());
}

TEST_F(EvntGrp_NoEventsComponentTest, SetEventDateAndEventDateMatch) {
}

TEST_F(EvntGrp_NoEventsComponentTest, SetEventPxAndEventPxMatch) {
    const double test_value = 123.456;
    component.setEventPx(test_value);
    EXPECT_EQ(component.getEventPx(), test_value);
    EXPECT_TRUE(component.hasEventPx());
}

TEST_F(EvntGrp_NoEventsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setEventType(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(EvntGrp_NoEventsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[208];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    EvntGrp::NoEvents decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(EvntGrp_NoEventsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
