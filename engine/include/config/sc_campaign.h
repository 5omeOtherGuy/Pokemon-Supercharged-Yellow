#ifndef GUARD_CONFIG_SC_CAMPAIGN_H
#define GUARD_CONFIG_SC_CAMPAIGN_H

// Owner test tools are opt-in and keep the ordinary campaign build unchanged.
#ifndef SC_TEST_TOOLS
#define SC_TEST_TOOLS 0
#endif

// Production Kanto rules. Ordinary upstream test fixtures keep their broad rules;
// use -DSC_TEST_CAMPAIGN=1 to test the production policy in a TESTING build.
#ifndef SC_TEST_CAMPAIGN
#define SC_TEST_CAMPAIGN 0
#endif
#if defined(FIRERED) && (!TESTING || SC_TEST_CAMPAIGN)
#define P_SC_KANTO_RULES 1
#else
#define P_SC_KANTO_RULES 0
#endif

#endif
