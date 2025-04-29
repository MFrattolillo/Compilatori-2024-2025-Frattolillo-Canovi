; ModuleID = 'Test/LoopInv2.c'
source_filename = "Test/LoopInv2.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @LoopInv2(i32 noundef %0, i32 noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  store i32 %0, ptr %3, align 4
  store i32 %1, ptr %4, align 4
  store i32 0, ptr %5, align 4
  store i32 3, ptr %6, align 4
  %10 = load i32, ptr %3, align 4
  %11 = load i32, ptr %4, align 4
  %12 = add nsw i32 %10, %11
  store i32 %12, ptr %7, align 4
  store i32 0, ptr %8, align 4
  br label %13

13:                                               ; preds = %24, %2
  %14 = load i32, ptr %8, align 4
  %15 = icmp slt i32 %14, 10
  br i1 %15, label %16, label %27

16:                                               ; preds = %13
  %17 = load i32, ptr %8, align 4
  %18 = mul nsw i32 %17, 2
  store i32 %18, ptr %9, align 4
  %19 = load i32, ptr %5, align 4
  %20 = load i32, ptr %9, align 4
  %21 = add nsw i32 %19, %20
  store i32 %21, ptr %5, align 4
  %22 = load i32, ptr %6, align 4
  %23 = mul nsw i32 %22, 2
  store i32 %23, ptr %6, align 4
  br label %24

24:                                               ; preds = %16
  %25 = load i32, ptr %8, align 4
  %26 = add nsw i32 %25, 1
  store i32 %26, ptr %8, align 4
  br label %13, !llvm.loop !6

27:                                               ; preds = %13
  %28 = load i32, ptr %5, align 4
  %29 = load i32, ptr %6, align 4
  %30 = add nsw i32 %28, %29
  %31 = load i32, ptr %7, align 4
  %32 = add nsw i32 %30, %31
  ret i32 %32
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 19.1.7 (/home/runner/work/llvm-project/llvm-project/clang cd708029e0b2869e80abe31ddb175f7c35361f90)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
