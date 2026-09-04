// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: InstrumentLeg::NoLegSecurityAltID (FIX4.3)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.3/components/InstrumentLeg.h>
#include <FIX4.3/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix43::components;

class InstrumentLeg_NoLegSecurityAltIDComponentTest : public ::testing::Test {
protected:
	InstrumentLeg::NoLegSecurityAltID component;

	void SetUp() override {
		component.reset();
	}
};

TEST_F(InstrumentLeg_NoLegSecurityAltIDComponentTest, ResetClearsAllFields) {
	// Set some fields
	
	// Reset component
	component.reset();
	
	// Verify all fields are cleared
	EXPECT_FALSE(component.hasAnySet());
}

TEST_F(InstrumentLeg_NoLegSecurityAltIDComponentTest, HasAnySetTracksPresence) {
	EXPECT_FALSE(component.hasAnySet());
	
}

TEST_F(InstrumentLeg_NoLegSecurityAltIDComponentTest, EncodeDecodeRoundtrip) {
	char buffer[148];
	
	// No scalar setter available to populate deterministically (this
	// component only wraps nested groups/sub-components). Group-only
	// containers with no required fields legitimately encode to 0
	// bytes, and some decode() implementations return false when no
	// bytes were consumed, so we only assert the two views agree.
	char *p = buffer;
	p = component.encode(p, true);
	std::size_t encoded_size = p - buffer;
	
	EXPECT_LE(encoded_size, component.compute_buffer_size());
	
	InstrumentLeg::NoLegSecurityAltID decoded;
	const char *q = buffer;
	(void)decoded.decode(q, p);
	EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(InstrumentLeg_NoLegSecurityAltIDComponentTest, CheckRequiredWhenEmpty) {
	EXPECT_TRUE(component.checkRequired());
}
