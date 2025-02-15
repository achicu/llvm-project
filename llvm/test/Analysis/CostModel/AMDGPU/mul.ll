; NOTE: Assertions have been autogenerated by utils/update_analyze_test_checks.py
; RUN: opt -cost-model -analyze -mtriple=amdgcn-unknown-amdhsa < %s | FileCheck -check-prefixes=ALL,SLOW16 %s
; RUN: opt -cost-model -analyze -mtriple=amdgcn-unknown-amdhsa -mcpu=gfx900 < %s | FileCheck -check-prefixes=ALL,FAST16 %s
; RUN: opt -cost-model -cost-kind=code-size -analyze -mtriple=amdgcn-unknown-amdhsa < %s | FileCheck -check-prefixes=ALL-SIZE,SLOW16-SIZE %s
; RUN: opt -cost-model -cost-kind=code-size -analyze -mtriple=amdgcn-unknown-amdhsa -mcpu=gfx900 < %s | FileCheck -check-prefixes=ALL-SIZE,FAST16-SIZE %s
; END.

define amdgpu_kernel void @mul_i32() #0 {
; ALL-LABEL: 'mul_i32'
; ALL-NEXT:  Cost Model: Found an estimated cost of 4 for instruction: %i32 = mul i32 undef, undef
; ALL-NEXT:  Cost Model: Found an estimated cost of 8 for instruction: %v2i32 = mul <2 x i32> undef, undef
; ALL-NEXT:  Cost Model: Found an estimated cost of 12 for instruction: %v3i32 = mul <3 x i32> undef, undef
; ALL-NEXT:  Cost Model: Found an estimated cost of 16 for instruction: %v4i32 = mul <4 x i32> undef, undef
; ALL-NEXT:  Cost Model: Found an estimated cost of 20 for instruction: %v5i32 = mul <5 x i32> undef, undef
; ALL-NEXT:  Cost Model: Found an estimated cost of 10 for instruction: ret void
;
; ALL-SIZE-LABEL: 'mul_i32'
; ALL-SIZE-NEXT:  Cost Model: Found an estimated cost of 2 for instruction: %i32 = mul i32 undef, undef
; ALL-SIZE-NEXT:  Cost Model: Found an estimated cost of 4 for instruction: %v2i32 = mul <2 x i32> undef, undef
; ALL-SIZE-NEXT:  Cost Model: Found an estimated cost of 6 for instruction: %v3i32 = mul <3 x i32> undef, undef
; ALL-SIZE-NEXT:  Cost Model: Found an estimated cost of 8 for instruction: %v4i32 = mul <4 x i32> undef, undef
; ALL-SIZE-NEXT:  Cost Model: Found an estimated cost of 10 for instruction: %v5i32 = mul <5 x i32> undef, undef
; ALL-SIZE-NEXT:  Cost Model: Found an estimated cost of 1 for instruction: ret void
;
  %i32 = mul i32 undef, undef
  %v2i32 = mul <2 x i32> undef, undef
  %v3i32 = mul <3 x i32> undef, undef
  %v4i32 = mul <4 x i32> undef, undef
  %v5i32 = mul <5 x i32> undef, undef
  ret void
}

define amdgpu_kernel void @mul_i64() #0 {
; ALL-LABEL: 'mul_i64'
; ALL-NEXT:  Cost Model: Found an estimated cost of 20 for instruction: %i64 = mul i64 undef, undef
; ALL-NEXT:  Cost Model: Found an estimated cost of 40 for instruction: %v2i64 = mul <2 x i64> undef, undef
; ALL-NEXT:  Cost Model: Found an estimated cost of 60 for instruction: %v3i64 = mul <3 x i64> undef, undef
; ALL-NEXT:  Cost Model: Found an estimated cost of 80 for instruction: %v4i64 = mul <4 x i64> undef, undef
; ALL-NEXT:  Cost Model: Found an estimated cost of 320 for instruction: %v8i64 = mul <8 x i64> undef, undef
; ALL-NEXT:  Cost Model: Found an estimated cost of 10 for instruction: ret void
;
; ALL-SIZE-LABEL: 'mul_i64'
; ALL-SIZE-NEXT:  Cost Model: Found an estimated cost of 12 for instruction: %i64 = mul i64 undef, undef
; ALL-SIZE-NEXT:  Cost Model: Found an estimated cost of 24 for instruction: %v2i64 = mul <2 x i64> undef, undef
; ALL-SIZE-NEXT:  Cost Model: Found an estimated cost of 36 for instruction: %v3i64 = mul <3 x i64> undef, undef
; ALL-SIZE-NEXT:  Cost Model: Found an estimated cost of 48 for instruction: %v4i64 = mul <4 x i64> undef, undef
; ALL-SIZE-NEXT:  Cost Model: Found an estimated cost of 192 for instruction: %v8i64 = mul <8 x i64> undef, undef
; ALL-SIZE-NEXT:  Cost Model: Found an estimated cost of 1 for instruction: ret void
;
  %i64 = mul i64 undef, undef
  %v2i64 = mul <2 x i64> undef, undef
  %v3i64 = mul <3 x i64> undef, undef
  %v4i64 = mul <4 x i64> undef, undef
  %v8i64 = mul <8 x i64> undef, undef
  ret void
}

define amdgpu_kernel void @mul_i16() #0 {
; SLOW16-LABEL: 'mul_i16'
; SLOW16-NEXT:  Cost Model: Found an estimated cost of 4 for instruction: %i16 = mul i16 undef, undef
; SLOW16-NEXT:  Cost Model: Found an estimated cost of 8 for instruction: %v2i16 = mul <2 x i16> undef, undef
; SLOW16-NEXT:  Cost Model: Found an estimated cost of 16 for instruction: %v3i16 = mul <3 x i16> undef, undef
; SLOW16-NEXT:  Cost Model: Found an estimated cost of 10 for instruction: ret void
;
; FAST16-LABEL: 'mul_i16'
; FAST16-NEXT:  Cost Model: Found an estimated cost of 4 for instruction: %i16 = mul i16 undef, undef
; FAST16-NEXT:  Cost Model: Found an estimated cost of 4 for instruction: %v2i16 = mul <2 x i16> undef, undef
; FAST16-NEXT:  Cost Model: Found an estimated cost of 8 for instruction: %v3i16 = mul <3 x i16> undef, undef
; FAST16-NEXT:  Cost Model: Found an estimated cost of 10 for instruction: ret void
;
; SLOW16-SIZE-LABEL: 'mul_i16'
; SLOW16-SIZE-NEXT:  Cost Model: Found an estimated cost of 2 for instruction: %i16 = mul i16 undef, undef
; SLOW16-SIZE-NEXT:  Cost Model: Found an estimated cost of 4 for instruction: %v2i16 = mul <2 x i16> undef, undef
; SLOW16-SIZE-NEXT:  Cost Model: Found an estimated cost of 8 for instruction: %v3i16 = mul <3 x i16> undef, undef
; SLOW16-SIZE-NEXT:  Cost Model: Found an estimated cost of 1 for instruction: ret void
;
; FAST16-SIZE-LABEL: 'mul_i16'
; FAST16-SIZE-NEXT:  Cost Model: Found an estimated cost of 2 for instruction: %i16 = mul i16 undef, undef
; FAST16-SIZE-NEXT:  Cost Model: Found an estimated cost of 2 for instruction: %v2i16 = mul <2 x i16> undef, undef
; FAST16-SIZE-NEXT:  Cost Model: Found an estimated cost of 4 for instruction: %v3i16 = mul <3 x i16> undef, undef
; FAST16-SIZE-NEXT:  Cost Model: Found an estimated cost of 1 for instruction: ret void
;
  %i16 = mul i16 undef, undef
  %v2i16 = mul <2 x i16> undef, undef
  %v3i16 = mul <3 x i16> undef, undef
  ret void
}

attributes #0 = { nounwind }
