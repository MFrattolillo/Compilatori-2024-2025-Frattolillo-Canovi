; ModuleID = 'test/loopFus.ll'
source_filename = "test/loopFus.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@__const.loopFusionExample.b = private unnamed_addr constant [5 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5], align 16

; Function Attrs: noinline nounwind uwtable
define dso_local void @loopFusionExample(i32 noundef %0) #0 {
  %2 = alloca [5 x i32], align 16
  %3 = alloca [5 x i32], align 16
  %4 = alloca [5 x i32], align 16
  call void @llvm.memset.p0.i64(ptr align 16 %2, i8 0, i64 20, i1 false)
  call void @llvm.memcpy.p0.p0.i64(ptr align 16 %3, ptr align 16 @__const.loopFusionExample.b, i64 20, i1 false)
  call void @llvm.memset.p0.i64(ptr align 16 %4, i8 0, i64 20, i1 false)
  br label %5

5:                                                ; preds = %13, %1
  %.01 = phi i32 [ 0, %1 ], [ %6, %13 ]
  %6 = add nsw i32 %.01, 1
  %7 = sext i32 %6 to i64
  %8 = getelementptr inbounds [5 x i32], ptr %3, i64 0, i64 %7
  %9 = load i32, ptr %8, align 4
  %10 = add nsw i32 %9, 1
  %11 = sext i32 %6 to i64
  %12 = getelementptr inbounds [5 x i32], ptr %2, i64 0, i64 %11
  store i32 %10, ptr %12, align 4
  br label %13

13:                                               ; preds = %5
  %14 = icmp slt i32 %6, %0
  br i1 %14, label %5, label %15, !llvm.loop !6

15:                                               ; preds = %13
  br label %16

16:                                               ; preds = %24, %15
  %.0 = phi i32 [ 0, %15 ], [ %17, %24 ]
  %17 = add nsw i32 %.0, 1
  %18 = sext i32 %17 to i64
  %19 = getelementptr inbounds [5 x i32], ptr %3, i64 0, i64 %18
  %20 = load i32, ptr %19, align 4
  %21 = mul nsw i32 %20, 2
  %22 = sext i32 %17 to i64
  %23 = getelementptr inbounds [5 x i32], ptr %4, i64 0, i64 %22
  store i32 %21, ptr %23, align 4
  br label %24

24:                                               ; preds = %16
  %25 = icmp slt i32 %17, %0
  br i1 %25, label %16, label %26, !llvm.loop !8

26:                                               ; preds = %24
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #1

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #2

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #2 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

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
!8 = distinct !{!8, !7}
